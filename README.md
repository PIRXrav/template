# template _[WIP]_

A simple generator to create custom template files for any application

### Installation
```
	./configure
	make
	sudo make install
```

### Usage

```
	template --help
```
### Exemple

```
	template src/kernel.c
```

### Configuration of templates
The templates are in `~/.template`.
Refer to `~/.template/templaterc.sh` for the management of `$names$`.
