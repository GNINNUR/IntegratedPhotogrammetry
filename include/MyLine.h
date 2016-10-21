#ifndef _My_LINE_STRUCT_H_
#define	_My_LINE_STRUCT_H_


#include "LidBaseDef.h"

//opengl�е�����ϵ��X���������ң�Y���������ϣ�Z����������
//�ѿ�������ϵ��X���������⣬Y���������ң�Z����������
//��ǰ����ϵ��X���������ң�Y���������⣬Z����������

#define	Square(x)	((x)*(x))

class CMyGLLine
{
public:
	CMyGLLine(D_3DOT dStart, D_3DOT dEnd);
	CMyGLLine();
	~CMyGLLine();

	void	SetLine(D_3DOT Pnt1, D_3DOT Pnt2);
	D_3DOT	GetPntByX(double x);
	D_3DOT	GetPntByY(double y);
	D_3DOT	GetPntByZ(double z);

	bool	IsOnLine(D_3DOT pt);	//�жϵ��Ƿ�λ���߶���
	bool	IsIntersectWith(CMyGLLine &LineSeg1, D_3DOT &IntersectPnt);
	
	void	GetLineSegLength(double	&dis);
	void	GetLineDirectionParameter(double &ax,double &ay,double &az);
	void	GetStartPoint(D_3DOT* pnt);
	void	GetEndPoint(D_3DOT* pnt);

protected:
	void	CalculateVectorParameter();


private:
	D_3DOT		m_StartPnt;
	D_3DOT		m_EndPnt;		//����ʽ��ʾ

	double		m_dDistance;	//������ģ�뷽�����ұ�ʾ
	double		m_cosx;
	double		m_cosy;
	double		m_cosz;
};


#endif