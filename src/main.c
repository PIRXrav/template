#define _DEFAULT_SOURCE
#include <argp.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMPLATE_DELIMITOR '$'

/***************** PATH ********************/

#define TEMPLATES_PATH_FROM_HOME ".template"
#define TEMPLATERC "templaterc.sh"

#define PATH_BUFFER_SIZE 1024
char template_path[PATH_BUFFER_SIZE];
char templaterc[PATH_BUFFER_SIZE];

void calc_path(void) {
  assert(snprintf(template_path, PATH_BUFFER_SIZE,
                  "%s/" TEMPLATES_PATH_FROM_HOME, getenv("HOME")));
  assert(
      snprintf(templaterc, PATH_BUFFER_SIZE, "%s/" TEMPLATERC, template_path));
}

/***************** ARGUMENTS ********************/

/* Program documentation. */
static char doc[] = "A template creator";

/* A description of the arguments we accept. */
static char args_doc[] = "NAME";

/* The options we understand. */
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {"format", 'f', "NAME", 0, "The desired language (ex : -f py)", 0},
    {"type", 't', "NUMBER", 0, "The template type (ex : -t 2)", 0},
    {0}};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *namefile; /* NAME */
  int verbose;
  char *format;
  long type;
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
  case 'v':
    arguments->verbose = 1;
    break;
  case 'f':
    arguments->format = arg;
    break;
  case 't': {
    char *endPtr;
    arguments->type = strtol(arg, &endPtr, 10);
    if (endPtr == arg)
      argp_usage(state);
    break;
  }
  case ARGP_KEY_ARG:
    if (state->arg_num == 1)
      /* Too many arguments. */
      argp_usage(state);
    arguments->namefile = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1)
      /* Not enough arguments. */
      argp_usage(state);
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/***************** CONFIG ********************/
// Parse .templaterc

struct tup_str {
  char *key;
  char *dat;
  struct tup_str *next;
};

struct tup_str *head = NULL;

void config_init(char *name) {
  FILE *frc = fopen(templaterc, "r");
  if (!frc) {
    perror("No template configuration file : " TEMPLATERC "\n");
    exit(1);
  }
  const size_t buff_size = 255;
  char buff[buff_size];
  size_t i_buff = 0;
  char ch;
  head = malloc(sizeof(struct tup_str));
  head->key = "NAME";
  head->dat = name;
  head->next = NULL;

  char buff_key[1024];
  char buff_dat[1024];
  char cmd[2048];

  while ((ch = fgetc(frc)) != EOF) {
    if (ch == '\n') {
      buff[i_buff] = '\0';
      i_buff = 0;
      // printf("%s\n", buff);
      if (sscanf(buff, " %[a-zA-Z0-9_] = %[^\n]", buff_key, buff_dat)) { // ok
        // printf("KEY : %s \nDAT : %s \n", buff_key, buff_dat);
        // Apply bash code on temp var
        char buff_out[1024];
        size_t i_buff_out = 0;
        sprintf(cmd, "temp=%s && echo $temp", buff_dat);
        FILE *fp = popen(cmd, "r");
        while ((ch = fgetc(fp)) != EOF) {
          if (ch == '\n')
            break;
          buff_out[i_buff_out++] = ch;
        }
        buff_out[i_buff_out++] = '\0';
        fclose(fp);
        // printf("OUT : %s\n", buff_out);
        // Create new node
        struct tup_str *new_head = calloc(1, sizeof(struct tup_str));
        assert(new_head);
        new_head->next = head;
        head = new_head;
        // add data
        head->key = strncpy(malloc(sizeof(strlen(buff_key) + 1)), buff_key,
                            strlen(buff_key) + 1);
        head->dat =
            strncpy(malloc(sizeof(strlen(buff_out) + 1)), buff_out, i_buff_out);
      }
    } else {

      assert(i_buff < buff_size);
      buff[i_buff++] = ch;
    }
  }
  fclose(frc);
}

char *config_get(char *key) {
  for (struct tup_str *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(key, cur->key) == 0) {
      return cur->dat;
    }
  }
  return "UNDEFINED IN" TEMPLATERC;
}

/***************** EDITOR ********************/
// Parse type.file and gen outfile.file

void editor_create(size_t type, char *format, char *name) {
  char inname[1024];
  assert(snprintf(inname, 1024, "%s/%ld.%s", template_path, type, format));
  char outname[1024];
  assert(snprintf(outname, 1024, "./%s.%s", name, format));
  /*
  printf("INPUT  : %s\n", inname);
  printf("OUTPUT : %s\n", outname);
*/
  FILE *fin = fopen(inname, "r");
  if (!fin) {
    fprintf(stderr, "No template %s\n", inname);
    exit(1);
  }
  // Test out file
  FILE *fout = fopen(outname, "r");
  if (fout) { // Existing
    fclose(fout);
    fprintf(stderr, "Existing file %s\n", outname);
    exit(1);
  }
  fout = fopen(outname, "w");
  assert(fout);

  const size_t buff_size = 255;
  char buff[buff_size];
  size_t i_buff = 0;
  char ch;
  int state = 0; // 0 : out $$; 1 : in $$
  while ((ch = fgetc(fin)) != EOF) {
    if (state == 0) { // out
      if (ch == TEMPLATE_DELIMITOR) {
        state = 1 - state;
      } else {
        fputc(ch, fout);
      }
    } else { // in
      if (ch == TEMPLATE_DELIMITOR) {
        state = 1 - state;
        buff[i_buff] = '\0';
        fputs(config_get(buff), fout);
        i_buff = 0;
      } else {
        assert(i_buff < buff_size);
        buff[i_buff++] = ch;
      }
    }
  }
  fclose(fin);
  fclose(fout);
}

int main(int argc, char **argv) {

  struct arguments arguments;
  /* Default values. */
  arguments.namefile = NULL;
  arguments.verbose = 0;
  arguments.format = "c";
  arguments.type = 0;
  static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  if (arguments.verbose) {
    printf("namefile %s\n", arguments.namefile);
    printf("verbose  %s\n", arguments.verbose ? "True" : "False");
    printf("template %ld.%s\n", arguments.type, arguments.format);
  }

  calc_path();
  if (arguments.verbose) {
    printf("PATH : %s\n", template_path);
    printf("rc.sh: %s\n", templaterc);
  }

  config_init(arguments.namefile);
  if (arguments.verbose) {
    for (struct tup_str *cur = head; cur != NULL; cur = cur->next) {
      printf("%s --> %s\n", cur->key, cur->dat);
    }
  }

  editor_create(arguments.type, arguments.format, arguments.namefile);

  if (arguments.verbose) {
    printf("DONE\n");
  }
  return 0;
}
