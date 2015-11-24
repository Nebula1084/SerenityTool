import sys
with open(sys.argv[1],'rb') as fin:
    fin.read(4)
    buf = fin.read()
with open('BankA.coe','w') as banka:
    with open('BankB.coe','w') as bankb:
        banka.write('memory_initialization_radix=16;\nmemory_initialization_vector=')
        bankb.write('memory_initialization_radix=16;\nmemory_initialization_vector=')
        for i in range(len(buf)):
            if i == 0:
                banka.write('\n%02X'%buf[i])
            elif i == 2:
                bankb.write('\n%02X'%buf[i])
            elif i%4 == 0:
                banka.write(',\n%02X'%buf[i])
            elif i%4 == 1:
                banka.write('%02X'%buf[i])
            elif i%4 == 2:
                bankb.write(',\n%02X'%buf[i])
            elif i%4 == 3:
                bankb.write('%02X'%buf[i])
        banka.write(';')
        bankb.write(';')
