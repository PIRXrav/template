#!/bin/sh

# prefix
prefix=/usr/local

# overwrite rc files
overwriterc=false

for arg in "$@"; do
    case "$arg" in
    --prefix=*)
        prefix=`echo $arg | sed 's/--prefix=//'`
        ;;

    --overwrite-rc)
        overwriterc=true
        ;;

    --help)
        echo 'usage: ./configure [options]'
        echo 'options:'
        echo '  --prefix=<path>: installation prefix'
        echo '  --overwrite-rc: overwrite ~/.template folder'
        echo 'all invalid options are silently ignored'
        exit 0
        ;;
    esac
done

echo "init ~/.template [overwrite: $overwriterc] ..."
if $overwriterc
then
  cp -v -r ./.template ~/
else
  cp --no-clobber -v -r ./.template ~/
fi

echo 'generating makefile ...'
echo "PREFIX = $prefix" >Makefile
cat Makefile.in >>Makefile

echo 'configuration complete, type make to build.'
