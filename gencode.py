import re
import copy
fin = open("InterCode1.txt","r")
s = fin.readlines()
lnum,rnum,defnum = map(int,s[0].split())
final_out = []

final_out = ['.data']
size_table = {}
def is_num(t):
    return t[0] <= '9' and t[0] >= '0'

def remove_useless_return(ts):
    tdel = [1 for i in range(0,len(ts)+5)]
    for i in range(1,len(ts)):
        if ts[i] == 'jr $ra' and ts[i-1] == 'jr $ra':
            tdel[i] = 0
    ts1 = ts
    ts = []
    for i in range(0,len(ts1)):
        if tdel[i] == 1:
            ts.append(ts1[i])
    return ts

def get_size(t):
    re = size_table[t]
    if re == None:
        return 1
    return re

ARRAYS = []

def newplace():
    global rnum
    rnum = rnum+1
    return rnum

# if has 2 spaces

pn = [0 for i in range(0,lnum+2)]
func = []
glob = []
st_variables = []

defs = s[1:defnum+1]
for i in range(0,defnum):
    ss = s[i+1+lnum].split(' ')
    if ss[0] == 'globaldef' or ss[0] == 'globaldefarr':
        if ss[1] == '$v0':
            continue
        sns = int(ss[2])
        sn = ss[1]
        size_table[sn] = sns
        glob.append(ss[1])
        if ss[0] == 'globaldefarr':
            ARRAYS.append(ss[1])
            final_out.append("%s: .space %d"%('G'+ss[1][1:],sns*4))
        else:
            final_out.append("%s: .word 0"%('G'+ss[1][1:]))
    if ss[0] == 'localdefarr':
        ARRAYS.append(ss[2])
final_out.append("")
final_out.append(".text")
final_out.append('.globl main')
final_out.append('main:\njal ACTUAL_MAIN\nli $v0,10\nsyscall')

flag = 0
st = ed = 0
ts = []

trans = {}
#sw a 0(b)   get-b
#else None
def get_mem_reg2(s):
    ss = s.split(' ')[2]
    if ss[len(ss)-1] != ')':
        return (s,None)
    for i in range(0,len(ss)):
        if ss[i] == '(':
            return (ss[:i],ss[i+1:len(ss)-1])
    return (s,None)

def transtoken(tok,st_variables):
    global rnum
    if tok in trans:
        return (trans[tok],[],[])
    if tok[0] == '$':
        return (tok,[],[])
    if tok in glob:
        n = rnum+1
        rnum = rnum+1
        n1= rnum+1
        ns = 'T%d'%n
        n1s = 'T%d'%n1
        rnum = rnum+1
        if tok in ARRAYS:
            return (ns,['la %s %s'%(ns,tok)],['la %s %s'%(ns,tok)])
        return (ns,['la %s %s'%(n1s,tok),'lw %s 0(%s)'%(ns,n1s)],['la %s %s'%(n1s,tok),'sw %s 0(%s)'%(ns,n1s)])
    if tok[0] == 'L':
        return (tok,[],[])
    if in_stack(tok,st_variables):
        n = rnum+1
        rnum = rnum+1
        n1= rnum+1
        ns = 'T%d'%n
        n1s = 'T%d'%n1
        rnum = rnum+1
        if tok in ARRAYS:
            return (ns,['la %s %s'%(ns,tok)],['la %s %s'%(ns,tok)])
        return (ns,['la %s %s'%(n1s,tok),'lw %s 0(%s)'%(ns,n1s)],['la %s %s'%(n1s,tok),'sw %s 0(%s)'%(ns,n1s)])
    if tok[0] == 't':
        return ('T'+tok[1:],[],[])
    return (tok,[],[])

pnum = 0
def is_if(t):
    return t == 'beq' or t == 'bne' or t == 'blt' or t == 'ble' or t == 'bgt' or t == 'bge';
def is_j(t):
    return t == 'syscall' or t == 'j' or t == 'jr' or t == 'jal' or t == 'Label'

def is_2op(t):
    return t == 'move' or t == 'li' or t == 'sw' or t == 'lw' or t == 'not'
def is_3op(t):
    return t == 'add' or t == 'sub' or t == 'mul' or t == 'div' or t == 'and' or t == 'or' or t == 'xor' or t == 'sll' or t == 'srl' or t == 'nor' or t == 'rem'
def transexp(line,st_variables):
    global pnum
    re = []
    ss = line.split(' ')
    n = len(ss)
    if ss[0] == 'Label':
        return [line]
    if ss[0] == 'lw' or ss[0] == 'sw':
        a,b,c = transtoken(ss[1],st_variables)
        s1,s2 = get_mem_reg2(line)
        if s2:
            #a1,b1,c1 = transtoken(s2,st_variables)
            #if ss[0] == 'sw':
            #    re.extend(b)
            #re.extend(b1)
            #re.append('%s %s %s(%s)'%(ss[0],a,s1,a1))
            #if ss[0] == 'lw':
            #    re.extend(c)
            #a1,b1,c1 = transtoken(s2,st_variables)
            if ss[0] == 'sw':
                re.extend(b)
            t1 = newplace()
            t2 = newplace()
            t1s = 'T%d'%t1
            t2s = 'T%d'%t2
            ttt = '0'
            bola = (s1[0] <= '9' and s1[0] >= '0')
            bolb = (not in_stack(s2,st_variables) and not (s2 in glob))
            if bola:
                rr = 4 * int(s1)
                t2s = '%d'%rr
                if bolb:
                    a3,b3,c3 = transtoken(s2,st_variables)
                    re.extend(b3)
                    t1s = a3
                else:
                    re.append('la %s %s'%(t1s,s2))
            else:
                a2,b2,c2 = transtoken(s1,st_variables)
                re.extend(b2)
                re.append('mul %s %s 4'%(t2s,a2))
                if bolb:#not global nor stack
                    a3,b3,c3 = transtoken(s2,st_variables)
                    re.extend(b3)
                    t1s = a3
                    re.append('add %s %s %s'%(t1s,t1s,t2s))
                    t2s = '0'
                else:
                    re.append('la %s %s'%(t1s,s2))
                    re.append('add %s %s %s'%(t1s,t1s,t2s))
                    t2s = '0'
            re.append("%s %s %s(%s)"%(ss[0],a,t2s,t1s))
            if ss[0] == 'lw':
                re.extend(c)
            return re
        else:
            if ss[0] == 'sw':
                re.extend(b)
            re.append('%s %s 0(%s)'%(ss[0],a,ss[2]))
            if ss[0] == 'lw':
                re.extend(c)
            return re
    if ss[0] == 'la':
        return [line]
    if ss[0] == 'read':
        a,b,c = transtoken(ss[1],st_variables)
        re.append('li $v0 5')
        re.append('syscall')
        re.append('move %s $v0'%a)
        re.extend(c)
        return re
    if ss[0] == 'write':
        a,b,c = transtoken(ss[1],st_variables)
        re.append('li $v0 1')
        re.extend(b)
        re.append('move $a0 %s'%a)
        re.append('syscall')
        return re
    if ss[0] != 'param':
        pnum = 0
    if ss[0] == 'return':
        return ['jr $ra']
    if ss[0] == 'goto':
        return ['j %s'%ss[1]]
    if ss[0] == 'param':
        a,b,c = transtoken(ss[1],st_variables)
        re.extend(b)
        re.append('move $a%d %s'%(pnum,a))
        pnum = pnum+1
        return re
    if ss[0] == 'j':
        return [line]
    if ss[0] == 'jr':
        return [line]
    if ss[0] == 'jal':
        return [line]
    if ss[0] == 'syscall':
        return [line]
    if ss[0] == 'return':
        return ['jr $ra']
    if ss[0] == 'call':
        return ['jal %s'%ss[1]]
    if is_if(ss[0]):
        a,b,c = transtoken(ss[1],st_variables)
        a1,b1,c1 = transtoken(ss[2],st_variables)
        re.extend(b)
        re.extend(b1)
        re.append('%s %s %s %s'%(ss[0],a,a1,ss[3]))
        return re
    if is_2op(ss[0]):
        a,b,c = transtoken(ss[1],st_variables)
        a1,b1,c1 = transtoken(ss[2],st_variables)
        #re.extend(b)
        re.extend(b1)
        re.append('%s %s %s'%(ss[0],a,a1))
        re.extend(c)
        return re
    if is_3op(ss[0]):
        a,b,c = transtoken(ss[1],st_variables)
        a1,b1,c1 = transtoken(ss[2],st_variables)
        a2,b2,c2 = transtoken(ss[3],st_variables)
        #re.extend(b)
        re.extend(b1)
        re.extend(b2)
        re.append('%s %s %s %s'%(ss[0],a,a1,a2))
        re.extend(c)
        return re
    return [line]

#list of free registers for this function
init_free_regs = ['$a%d'%i for i in range(0,4)]+['$t%d'%i for i in range(0,10)]+['$s%d'%i for i in range(0,8)]+['$v%d'%i for i in range(0,2)]
free_regs = []
def get_type(s):
    return s.split(' ')[0]

def divide(func):
    n = len(func)
    is_leader = [0 for i in range(0,n+1)]
    is_leader[n] = 1
    is_leader[0] = 1
    Label_block = {}
    block = []
    block_num = [0 for i in range(0,n)]
    next_block = [[] for i in range(0,n)]
    prev_block = [[] for i in range(0,n)]
    st = 0
    for i in range(0,n):
        ts = get_type(func[i])
        if is_if(ts) or ts == 'j' or ts == 'jr' or ts == 'jal' or ts == 'syscall':
            is_leader[i+1] = 1
        if ts == 'Label' :
            is_leader[i] = 1
            tt = func[i].split(' ')[1][:-1]
            Label_block[tt] = i
        if is_leader[i]:
            if i == 0:
                None
            else:
                block.append((st,i-1))
                st = i
        if i == 0:
            block_num[i] = 0
        else:
            block_num[i] = is_leader[i] + block_num[i-1]
        if ts == 'Label':
            tt = func[i].split(' ')[1][:-1]
            Label_block[tt] = block_num[i]
    for i in range(0,n):
        ts = get_type(func[i])
        cbl = block_num[i]
        if is_if(ts) or ts == 'j':
            tt = 0
            if ts == 'j': 
                tt = func[i].split(' ')[1]
            else:
                tt = func[i].split(' ')[3]
            tbl = Label_block[tt]
            next_block[cbl].append(tbl)
            prev_block[tbl].append(cbl)
    if st <= i-1:
        block.append((st,i-1))
    bnum = len(block)
    for i in range(0,bnum-1):
        ss = func[block[i+1][1]-1].split(' ')[0]
        if ss != 'jr':
            next_block[i].append(i+1)
            prev_block[i+1].append(i)
    for i in range(0,bnum):
        next_block[i] = list(set(next_block[i]))
        prev_block[i] = list(set(prev_block[i]))
    #print 'is_leader',is_leader
    #print 'Label_block',Label_block
    #print 'block',block
    #print 'block_num',block_num
    #print 'next_block',next_block
    #print 'prev_block',prev_block
    return (block,block_num,next_block,prev_block)

def in_stack(var,st_variables):
    if len(st_variables) == 0:
        return False
    for x,y in st_variables:
        if x == var:
            return True
    return False

def merge(s1,s2):
    return list(set(s1+s2))

def is_temp(r):
    return r[0] == 'T'
def cal_vars(s,st_variables,st):
    nl = []
    vl = []
    stt = {}
    edt = {}
    i = 0
    for line in s:
        ss = line.split(' ')
        n = len(ss)
        if ss[0] == 'lw' or ss[0] == 'sw':
            s1,s2 = get_mem_reg2(line)
            if ss[0] == 'sw':
                if not (ss[1] in vl) and is_temp(ss[1]):
                    nl.append(ss[1])
                    stt[ss[1]] = -1
                if s2 != None:
                    if is_temp(s2):
                        if not(s2 in vl):
                            nl.append(s2)
                            stt[s2] = -1
                        vl.append(s2)
                        if not (s2 in stt):
                            stt[s2] = i + st
                        edt[s2] = i + st
            if ss[0] == 'lw':
                if s2 != None:
                    if is_temp(s2):
                        if not(s2 in vl):
                            nl.append(s2)
                            stt[s2] = -1
                        vl.append(s2)
                        if not (s2 in stt):
                            stt[s2] = i + st
                        edt[s2] = i + st
            #   if not (ss[1] in stt) and is_temp(ss[1]):
            #        stt[ss[1]] = i+st
            if is_temp(ss[1]):
                vl.append(ss[1])
                if not (ss[1] in stt):
                    stt[ss[1]] = i + st
                edt[ss[1]] = i+st
        elif is_j(ss[0]):
            None
        elif is_if(ss[0]):
            if is_temp(ss[1]):
                if not (ss[1] in vl):
                    nl.append(ss[1])
                    stt[ss[1]] = -1
            if is_temp(ss[2]):
                if not (ss[2] in vl):
                    nl.append(ss[2])
                    stt[ss[2]] = -1
            if is_temp(ss[1]):
                vl.append(ss[1])
                if not (ss[1] in stt):
                    stt[ss[1]] = i + st
                edt[ss[1]] = i+st
            if is_temp(ss[2]):
                vl.append(ss[2])
                if not (ss[2] in stt):
                    stt[ss[2]] = i + st
                edt[ss[2]] = i+st
        elif ss[0] == 'la':
            if is_temp(ss[1]):
                stt[ss[1]] = i+st
                vl.append(ss[1])
                if not (ss[1] in stt):
                    stt[ss[1]] = i + st
                edt[ss[1]] = i + st
        elif is_2op(ss[0]):
            if is_temp(ss[2]):
                if not(ss[2] in vl):
                    nl.append(ss[2])
                    stt[ss[2]] = -1
            if is_temp(ss[1]):
                if not (ss[1] in stt):
                    stt[ss[1]] = i+st
            if is_temp(ss[1]):
                vl.append(ss[1])
                if not (ss[1] in stt):
                    stt[ss[1]] = i + st
                edt[ss[1]] = i+st
            if is_temp(ss[2]):
                vl.append(ss[2])
                if not (ss[2] in stt):
                    stt[ss[2]] = i + st
                edt[ss[2]] = i+st
        elif is_3op(ss[0]):
            if is_temp(ss[2]):
                if not(ss[2] in vl):
                    nl.append(ss[2])
                    stt[ss[2]] = -1
            if is_temp(ss[3]):
                if not(ss[3] in vl):
                    nl.append(ss[3])
                    stt[ss[3]] = -1
            if is_temp(ss[1]):
                if not (ss[1] in stt):
                    stt[ss[1]] = i+st
            if is_temp(ss[1]):
                vl.append(ss[1])
                if not (ss[1] in stt):
                    stt[ss[1]] = i + st
                edt[ss[1]] = i+st
            if is_temp(ss[2]):
                vl.append(ss[2])
                if not (ss[2] in stt):
                    stt[ss[2]] = i + st
                edt[ss[2]] = i+st
            if is_temp(ss[3]):
                vl.append(ss[3])
                if not (ss[3] in stt):
                    stt[ss[3]] = i + st
                edt[ss[3]] = i+st
        i = i + 1
    #print edt
    nl = list(set(nl))
    vl = list(set(vl))
    return (nl,vl,stt,edt)


nextlist = [[] for i in range(0,1000)]
prev_list = [[] for i in range(0,1000)]

def bfs_list(next_block,prev_block):
    bnum = len(next_block)
    for x in range(0,bnum):
        i = 0
        while i < len(next_block[x]):
            bn = next_block[x][i]
            for t in next_block[bn]:
                if not (t in next_block[x]):
                    next_block[x].append(t)
            i = i+1
        i = 0
        while i < len(prev_block[x]):
            bn = prev_block[x][i]
            for t in prev_block[bn]:
                if not (t in prev_block[x]):
                    prev_block[x].append(t)
            i = i+1
    for i in range(0,bnum):
        next_block[i] = list(set(next_block[i]))
        prev_block[i] = list(set(prev_block[i]))


def range_analyse(func,st_variables):
    func1 = []
    for line in func:
        func1.extend(transexp(line,st_variables))
    block,block_num,next_block,prev_block = divide(func1)
    bfs_list(next_block,prev_block)
    #print 'new next:',next_block
    #print 'new prev:',prev_block
    
    st_table = []#st and ed pos for every var at every block
    ed_table = []
    needlist = []
    varlist = []
    bnum = len(block)
    #print 'bnum = ',bnum
    for st,ed in block:
    #    print st,ed
    #    print func1[st:ed+1]
        t1,t2,dicst,diced = cal_vars(func1[st:ed+1],st_variables,st)
        st_table.append(dicst)
        ed_table.append(diced)
        needlist.append(t1)
        varlist.append(t2)
    #print 'st_table', st_table
    #print 'ed_table', ed_table
    #print 'varlist,' , varlist
    #print 'needlist,' , needlist
    
    totvarlist = reduce(merge,varlist)
    range_var = []
    for var in totvarlist:
    #    print var
        var_st = -1
        var_ed = -1
        for i in range(0,bnum):
            in_start = False
            in_need= False
            for x in prev_block[i]:
                if var in varlist[x]:
                    in_start = True
                    break
            for x in next_block[i]:
                if var in needlist[x]:
                    in_need= True
                    break
            if var in needlist[i]:
                in_need = True
                break
            if in_start and in_need:
                var_st = block[i][0]
                break
            if var in varlist[i]:
                var_st = st_table[i][var]
                break
        
        for i in range(bnum-1,-1,-1):
            in_end = False
            in_need = False
            for x in next_block[i]:
                if var in needlist[x]:
                    in_need= True
                    break
            if in_need:
                var_ed = block[i][1]
                break
            if var in varlist[i]:
                var_ed = ed_table[i][var]
                break
        range_var.append((var,var_st,var_ed))
    s_range_var = filter(lambda x:block_num[x[1]] != block_num[x[2]],range_var)
    t_range_var = filter(lambda x:block_num[x[1]] == block_num[x[2]],range_var)
    return (s_range_var,t_range_var)
    

class Error:
    None

def assign(func,st_variables):
    cont = True
    s_var = []
    t_var = []
    s_res = []
    while cont:
        s_var,t_var = range_analyse(func,st_variables)
        s_res,new_st_variables = s_assign(s_var,['$s%d'%i for i in range(0,8)])
        if len(new_st_variables) > 0:
            st_variables.extend(new_st_variables)
            re1 = []
            for i in range(0,len(func)):
                re1.extend(transexp(func[i],st_variables))
            func = re1
            #cont = False
        else:
            cont = False

    cont = True
    t_res = []
    cnt = 0
    while cont:
        s_var,t_var = range_analyse(func,st_variables)
        t_res,new_st_variables = s_assign(t_var,['$t%d'%i for i in range(0,10)])
        if len(new_st_variables):
            print 'register assign failed: too many temp variables'
            st_variables.extend(new_st_variables)
            re1 = []
            for i in range(0,len(func)):
                re1.extend(transexp(func[i],st_variables))
            func = re1
        else:
            cont = False
    return s_res,t_res,st_variables


def s_assign(s_var,free_list):
    #active elements (varname,startline,endline,register)
    #s_var elements (varname,startline,endline)
    s_res = []
    new_st_variables = []
    s_var = sorted(s_var,key=lambda x:x[1])
    active = []
    for i in range(0,len(s_var)):
        #expire old intervals
        while len(active) > 0 and active[0][2] < s_var[i][1]:
            free_list.append(active[0][3])
            s_res.append((active[0][0],active[0][3]))
            del active[0]

        #begin
        if len(free_list) == 0:
            spill = active[len(active)-1]
            if spill[2] > s_var[i][2]:
                del active[len(active)-1]
                new_st_variables.append((spill[0],1))
                active.append((s_var[i][0],s_var[i][1],s_var[i][2],spill[3]))
                active = sorted(active,key=lambda x:x[2])
            else:
                new_st_variables.append((s_var[i][0],1))
        else:
            active.append((s_var[i][0],s_var[i][1],s_var[i][2],free_list[0]))
            active = sorted(active,key=lambda x:x[2])
            del free_list[0]
            
    for i in range(0,len(active)):
        s_res.append((active[i][0],active[i][3]))
    return (s_res,new_st_variables)






####needlist//in - this - out
####varlist// this - out
####first_occur
####last_occur
####
def trans_final_token(tok):
    if is_num(tok):
        return tok
    if tok in glob:
        return 'G'+tok[1:]
    if tok in trans:
        return trans[tok]
    return tok
        

def trans_final_exp(line):
    ss = line.split(' ')
    #print line
    if ss[0] == 'lw' or ss[0] == 'sw':
        s1,s2 = get_mem_reg2(line)
        return ['%s %s,%s(%s)'%(ss[0],trans_final_token(ss[1]),trans_final_token(s1),trans_final_token(s2))]
    if is_2op(ss[0]) or ss[0] == 'la'or ss[0] == 'li':
        return ['%s %s,%s'%(ss[0],trans_final_token(ss[1]),trans_final_token(ss[2]))]
    if is_if(ss[0]):
        return ['%s %s,%s,%s'%(ss[0],trans_final_token(ss[1]),trans_final_token(ss[2]),trans_final_token(ss[3]))]
    if is_3op(ss[0]):
        return ['%s %s,%s,%s'%(ss[0],trans_final_token(ss[1]),trans_final_token(ss[2]),trans_final_token(ss[3]))]
    if ss[0] == 'jr':
        return before_return_instructions+['jr $ra']
    if ss[0] == 'Label':
        return [ss[1]]
    return [line]
    

finalfunc = []
before_return_instructions = []
def trans_final_func(func,s_res,t_res,st_variables):
    global before_return_instructions
    before_return_instructions = []
    res = []
    if func[0] == 'main:':
        res.append("ACTUAL_MAIN:")
    else:
        res.append(func[0])
    res.append("addi $sp,$sp,")
    ssize = 0
    for i in range(0,8):
        res.append("sw $s%d,%d($sp)"%(i,ssize))
        before_return_instructions.append('lw $s%d,%d($sp)'%(i,ssize))
        ssize = ssize + 4
    for i in range(0,4):
        res.append("sw $a%d,%d($sp)"%(i,ssize))
        before_return_instructions.append('lw $a%d,%d($sp)'%(i,ssize))
        ssize = ssize+4
    res.append("sw $ra,%d($sp)"%ssize)
    before_return_instructions.append('lw $ra,%d($sp)'%ssize)
    ssize = ssize+4
    for name,size in st_variables:
        trans[name] = '%d($sp)'%ssize
        ssize = ssize+size * 4
    res[1] = res[1] + '-%d'%ssize
    before_return_instructions.append("addi $sp,$sp,%d"%ssize)
    for name,reg in s_res:
        trans[name] = reg
    for name,reg in t_res:
        trans[name] = reg
    for line in func[1:]:
        res.extend(trans_final_exp(line))
    return res

def get_st_variables_for_func(funcname):
    #defs = s[1:defnum+1]
    re = []
    for i in range(0,defnum):
        ss = s[i+1+lnum].split(' ')
        if ss[0] == 'localdef' and ss[1] == funcname:
            re.append((ss[2],1))
            size_table[ss[2]] = 1
        if ss[0] == 'localdefarr' and ss[1] == funcname:
            re.append((ss[2],int(ss[3])))
            size_table[ss[2]] = ss[3]
            ARRAYS.append(ss[2])
    return re

def genFunc(s,st,ed):
    global trans,free_regs,init_free_regs
    re = []
    s0s = s[st].split(' ')
    argnum = int(s0s[2])
    arglist = []
    trans = {} 
    ret = s0s[3]
    free_regs = copy.deepcopy(init_free_regs)
    re.append(s0s[1]+':')
    for i in range(0,argnum):
        trans[s[st+1+i]] = 'T%d'%newplace()
        re.append('move T%d $a%d'%(rnum,i))
        free_regs.remove('$a%d'%i)
    trans[ret] = '$v0'
    free_regs.remove('$v0')
    if '$a0' in free_regs:
        free_regs.remove('$a0')
    st_variables = get_st_variables_for_func(s0s[1])
    for i in range(st+argnum+1,ed):
        re.extend(transexp(s[i],st_variables))
    re.extend(transexp('return',st_variables))
    s_res,t_res,st_variables = assign(re[1:],st_variables)
    re = remove_useless_return(re)
    finalfunc = trans_final_func(re,s_res,t_res,st_variables)
    final_out.extend(finalfunc)
    return re

for i in range(1,lnum+1):
    s[i] = s[i].strip().replace(',',' ')
    ss = s[i].split(' ')
    if not flag:
        if ss[0] == 'deffunc':
            flag = 1
            st = i
        else:
            None
    else:
        if ss[0] == 'endfunc':
            ts.extend(genFunc(s,st,i))
            flag = 0
        else:
            None

fout = open("Intercode2.txt","w")


ts = remove_useless_return(ts)
fout.write("%d\n"%defnum)
for i in range(1+lnum,1+lnum+defnum):
    fout.write(s[i])
fout.write("%d %d\n"%(len(ts),rnum))
for line in ts:
    fout.write(line+'\n')
fout.close()

fout = open("finalcode.s","w")
for line in final_out:
    fout.write(line+'\n')
fout.close()
