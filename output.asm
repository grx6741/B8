.text
ldi A 1
mov M A 255; M[a] = 'A
mov A M 255; 'A = M[a]
out 0; out(a)
ldi A 3
mov M A 254; M[b] = 'A
mov A M 254; 'A = M[b]
out 0; out(b)
hlt
