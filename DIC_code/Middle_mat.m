function [middle_mat] = Middle_mat(refer_subset,r)
%�����deltaP��ߵľ��� -��J^T*J��^-1*J^T ���� ���ش˾���
%   �˴���ʾ��ϸ˵��


%���ݶȾ���
gra_x=zeros(2*r+1,2*r+1);
gra_y=zeros(2*r+1,2*r+1);
%������x�����ݶȾ���
for m=1:2*r+1
    row=refer_subset(m,:);
    gra=gradient(row,2*r+1);
    gra_x(m,:)=gra;
end
%������y�����ݶȾ���
for n=1:2*r+1
    col=refer_subset(:,n);
    gra=gradient(col,2*r+1);
    gra_y(:,n)=gra;
end
i=1;


for patch_m=1:2*r+1   
    for patch_n=1:2*r+1
        G=[gra_x(patch_m,patch_n),gra_y(patch_m,patch_n)]; 
        x=patch_n-(r+1);
        y=patch_m-(r+1);
        J(i,:)=G*[1,x,y,0,0,0;0,0,0,1,x,y];
        %hessian_mat=hessian_mat+(G*[1,x,y,0,0,0;0,0,0,1,x,y])'*(G*[1,x,y,0,0,0;0,0,0,1,x,y]);
        i=i+1;
    end
end




% ����Hessian����


middle_mat=-inv((J'*J))*J';



end

