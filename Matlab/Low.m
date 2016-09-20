function Low(n)
global image set csind setlum k undo adj cset activemin activemax ddpcounter ddp
newactivemin=[];
for u=1:size(activemin,1)
    k=activemin(u);
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
            if minv>setlum
                ddpcounter=ddpcounter+1;
                ddp(ddpcounter).set=set;
                ddp(ddpcounter).rellum=setlum-minv;
                ddp(ddpcounter).cumlum=setlum;
                for s=1:size(set,1)
                    image(set(s,1),set(s,2))=minv;
                end
                setlum=minv;
                sz=size(set,1);
            else
                sz=inf;
                if maxv<setlum
                    activemax=[activemax;k];          
                else
                    cset(k).alife=0;
                end
            end
        end
        cset(k).set=set;
        if sz<inf
            newactivemin=[newactivemin;k];        
        end
    end
end
activemin=newactivemin;