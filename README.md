# WORKSPACE DE VITIS 2024.1 MICROBLAZE V
Este repositorio contiene un workspace de Vitis 2024.1 donde se han creado los siguientes componentes:
- Componente de plataforma: platform_microblaze_riscv, con un XSA de un Microblaze V con UART y AXI GPIOs, entre otros periféricos.
- Componente de software hello_world: ejemplo sencillo programado en C de una aplicación para el Microblaze V.
- Componente de software hello_world_BRAM: el mismo ejemplo de software anterior pero configurado para reducir su tamaño e incluirse y ejecutarse desde las BRAM del Microblaze V en lugar de la memoria DDR3.

Además, en la carpeta PERTE se puede encuentrar documentación y otros ficheros fuente con distintos ejemplos que utilizan el Microblaze V así como sus periféricos. Estos ficheros fuente se pueden probar sustituyendo el fichero hello_world.c de cualquiera de los componentes de software del workspace con el fichero .c de la carpeta de Ejemplos.
