import time
f = open("buff.bin", "wb")

bt = bytearray(5400000)
bt[0] = 2
bt[1] = 100
bt[2] = 255
f.write(bt)

f.close()

b = open("buff.bin", "rb")

t = time.time()
inn = bytearray(b.read(3))
end = time.time()
print(inn[0])
print(end-t)

b.close