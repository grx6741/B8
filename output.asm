.text
ldi A 0
mov M A 255; M[a] = 'A
ldi A 1
mov M A 254; M[b] = 'A
__loop0:
mov A M 255; 'A = M[a]
out 0; out(a)
mov A M 255; 'A = M[a]
mov B M 254; 'B = M[b]
add
mov M A 253; M[c] = 'A
mov A M 255; 'A = a
mov M A 254; M[b] = 'A
mov A M 253; 'A = c
mov M A 255; M[a] = 'A
mov A M 255; 'A = M[a]
ldi B 100
cmp
je %__else0
jc %__else0
__if0:
jmp %__endloop0
jmp %__endif0
__else0:
__endif0:
jmp %__loop0
__endloop0:
hlt
