function [gra] = gradient(Y,N)
%GRADIENT ����һά����Y�ͳ���N�������ݶ����� ����������ֵ��
DM=zeros(N,N);
D=zeros(N,1);
for i=2:N-1
    DM(i,i-1:i+1)=[1,4,1];
    D(i)=3*Y(i+1)-3*Y(i-1);
end

DM(1,[1 2])=[2,1];
D(1)=3*Y(2)-3*Y(1);
i=N;
DM(N,[N-1 N])=[1,2];
D(N)=3*Y(N)-3*Y(N-1);
%matlab���ݶȾ���Ҳ������׷�Ϸ�
gra=DM\D;
end

