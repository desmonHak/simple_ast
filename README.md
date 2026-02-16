# SIMPLE AST

----

Una implementacion de un AST simple en C puro que permite calcular el valor de una expresion matematica.

compile con:
```c
git submodule update --init --recursive
gcc main.c -o main.exe
```

Si usa linux, posiblemente deba compilar añadiendo la flag ``-D_GNU_SOURCE`` para no obtener un error de compilacion.

# Usar valgrind

```bash
# instalacion
sudo apt install valgrind

# Compila con debug info (-g)
gcc -g -Wall -o main main.c

# Ejecuta con Valgrind (memcheck por defecto)
valgrind --leak-check=full ./main

# Opcion mas util:
valgrind --leak-check=full \
         --leak-resolution=high \
         --num-callers=25 \
         --track-origins=yes \
         --show-leak-kinds=all \
         --show-reachable=yes \
         --log-file=valgrind-%p.log \
         ./main
```

**Explicación**:
- ``--leak-check=full``: Detalla dónde están los leaks
- ``--track-origins=yes``: Rastrea memoria no inicializada
- ``--log-file``: Guarda en archivo (útil si hay mucho output)
- ``--track-origins=yes``: Rastrea dónde se creó memoria no inicializada
- ``--show-reachable=yes``: También muestra memoria "still reachable"
- ``--suppressions=supp_file.txt``: Ignora leaks conocidos (librerías)

**Opciones específicas para ignorar falsos positivos**:
```C
--undef-value-errors=no       # Ignora valores no inicializados (más rápido)
--partial-loads-ok=yes        # Tolera cargas parciales en structs
```

----