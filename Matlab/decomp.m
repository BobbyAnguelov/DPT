%global image set csind setlum k undo adj cset activemin activemax N M ddpcounter dupcounter ddp dup
%t0=cputime;
%primecsets
%t1=cputime-t0
ddp=[];
dup=[];
ddpcounter=0;
dupcounter=0;
n=0;
while size(activemin,1)>0|size(activemax,1)>0
    n=n+1;
    Low(n)
    Upp(n)
end
t2=cputime-t1
t=t2+t1