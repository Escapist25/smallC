import re
fin = open("InterCode.txt","r")
fout = open("InterCode1.txt","w")

s = fin.readlines()
lnum,rnum = map(int,s[0].split())

next_quad = 0
def newquad():
    global next_quad
    next_quad = next_quad+1
    return next_quad

quad = [0 for i in range(1,lnum+5)]
used = [0 for i in range(1,lnum+5)]
deflist = []
to_main = []
in_func = 0
funcname = 0
for i in range(1,lnum+1):
    ss = s[i].split(' ')
    if not in_func:
        if ss[0] == 'deffunc':
            quad[i] = newquad()
            if ss[1] == 'main':
                next_quad += len(to_main)
            funcname = ss[1]
            in_func = 1
        else:
            if ss[0] == 'def':
                deflist.append('global'+s[i])
                quad[i] = next_quad+1
            elif ss[0] == 'defarr':
                deflist.append('global'+s[i])
                quad[i] = next_quad+1
            else:
                to_main.append(s[i])
    else:
        if ss[0] == 'def':
            s[i] = s[i][:3] + ' %s'%funcname + s[i][3:]
            deflist.append('local'+s[i])
            quad[i] = next_quad+1
        elif ss[0] == 'defarr':
            s[i] = s[i][:6] + ' %s'%funcname + s[i][6:]
            deflist.append('local'+s[i])
            quad[i] = next_quad+1
        else:
            quad[i] = newquad()
            if ss[0] == 'endfunc':
                in_func = 0
            

def is_if(t):
    return t == 'beq' or t == 'bne' or t == 'blt' or t == 'ble' or t == 'bgt' or t == 'bge';
for i in range(1,lnum+1):
    ss = s[i].split(' ')
    if is_if(ss[0]):
        used[quad[int(ss[4])]] = 1
    elif ss[0] == 'goto':
        used[quad[int(ss[1])]] = 1
    
ss1 = []
for i in range(1,lnum+1):
    ss = s[i].split(' ')
    if not in_func:
        if ss[0] == 'deffunc':
            ss1.append(s[i])
            quad[i] = newquad()
            if ss[1] == 'main':
                ss1.extend(to_main)
            in_func = 1
        else:
            None
    else:
        if ss[0] == 'def' or ss[0] == 'defarr' or ss[0] == 'localdef' or ss[0] == 'localdefarr' or ss[0] == 'globaldef' or ss[0] == 'globaldefarr':
            None
        else:
            if used[quad[i]] == 1:
                lnum+=1
                ss1.append("Label L%d:\n"%quad[i]);
            if is_if(ss[0]):
                ss[4] = 'L%d'%quad[int(ss[4])]
                del ss[3]
                ss1.append(' '.join(ss)+'\n')
            elif ss[0] == 'goto':
                ss[1] = 'L%d'%quad[int(ss[1])]
                ss1.append(' '.join(ss)+'\n')
            else:
                ss1.append(s[i])

            if ss[0] == 'endfunc':
                in_func = 0

ss2 = ['%d %d %d\n'%(lnum-len(deflist),rnum,len(deflist))]
ss2.extend(ss1)
for line in ss2:
    fout.write(line.replace(',' ,' '))
for line in deflist:
    fout.write(line)
fout.close()
    
