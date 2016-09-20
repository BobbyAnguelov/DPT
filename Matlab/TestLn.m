t0=cputime;
primecsets
t1=cputime-t0
ddpcounter=0;
dupcounter=0;
for n=1:40000
    Lower(n)
    Upper(n)
end
t2=cputime-t1
t=t2+t1