function Upp(n)
global image set csind setlum k undo adj cset activemin activemax dupcounter dup
newactivemax=[];
for u=1:size(activemax,1)
    k=activemax(u);
    if cset(k).alife>0
        set=cset(k).set; 
        setlum=image(set(1,1),set(1,2)); 
        sz=size(set,1);
        while sz<=n
            adj=[]; undo=[]; 
            grow(cset(k).adj);
            for s=1:size(undo,1)
                csind(adj(s,1),adj(s,2))=undo(s,1);
            end
            cset(k).adj=adj;
            [minv,maxv]=range(image,adj);
            if maxv<setlum
                dupcounter=dupcounter+1;
                dup(dupcounter).set=set;
                dup(dupcounter).rellum=setlum-maxv;
                dup(dupcounter).cumlum=setlum;
                for s=1:size(set,1)
                    image(set(s,1),set(s,2))=maxv;
                end
                setlum=maxv;
                sz=size(set,1);
            else
                sz=inf;
                if minv>setlum
                    activemin=[activemin;k];          
                else
                    cset(k).alife=0;
                end
            end
        end
        cset(k).set=set;
        if sz<inf
            newactivemax=[newactivemax;k];        
        end
    end
end
activemax=newactivemax;