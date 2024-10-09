.section .text
.globl __bswapsi2
.type __bswapsi2, @function

__bswapsi2:
    # a0 contiene el valor de entrada

    # Intercambiar los bytes individuales
    slli    t0, a0, 24          # Desplaza el byte menos significativo 24 bits a la izquierda
    srli    t1, a0, 24          # Desplaza el byte más significativo 24 bits a la derecha

    # Intercambiar los bytes en el medio
    slli    t2, a0, 8           # Desplaza el segundo byte 8 bits a la izquierda
    li      t3, 0x00FF0000      # Cargar la máscara en t3
    and     t2, t2, t3          # Aplica la máscara para aislar los bits relevantes

    srli    t3, a0, 8           # Desplaza el tercer byte 8 bits a la derecha
    li      t4, 0x0000FF00      # Cargar la máscara en t4
    and     t3, t3, t4          # Aplica la máscara para aislar los bits relevantes

    # Combinar todos los bytes en el registro de salida
    or      t0, t0, t1          # Combina el byte menos significativo con el más significativo
    or      t0, t0, t2          # Añade el segundo byte
    or      t0, t0, t3          # Añade el tercer byte

    # Retornar el valor intercambiado
    mv      a0, t0
    ret
