
#include "PIC_RGB.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


CPIC_RGB::CPIC_RGB()
{
	base = NULL;
	w = 0;
	h = 0;
	Line_Span = 0;
	time_Stamp = 0;
}

CPIC_RGB::~CPIC_RGB()
{
	if(base != NULL)
	{
		free(base);
	}
}

bool CPIC_RGB::GetR(CPIC_RGB * ret)
{
	if(ret == NULL)
	{
		return false;
	}
	if(ret->w != this->w ||ret->h != this->h  )
	{
		return false;
	}
	long num = w*h;
	for(int i = 0; i < num ; i ++)
	{
		
		ret->base[i].r = base[i].r ;
		ret->base[i].g = 0;
		ret->base[i].b =0;
		
	}
	return true;
}

bool CPIC_RGB::GetG(CPIC_RGB * ret)
{
	if(ret == NULL)
	{
		return false;
	}
	if(ret->w != this->w ||ret->h != this->h  )
	{
		return false;
	}
	long num = w*h;
	for(int i = 0; i < num ; i ++)
	{
		
		ret->base[i].r = 0 ;
		ret->base[i].g = base[i].g;
		ret->base[i].b =0;
		
	}
	return true;
}

bool CPIC_RGB::GetB(CPIC_RGB * ret)
{
	if(ret == NULL)
	{
		return false;
	}
	if(ret->w != this->w ||ret->h != this->h  )
	{
		return false;
	}
	long num = w*h;
	for(int i = 0; i < num ; i ++)
	{
		
		ret->base[i].r = 0 ;
		ret->base[i].g = 0;
		ret->base[i].b =base[i].b;
		
	}
	return true;
}




bool CPIC_RGB::GetR_F(CPIC_RGB * ret)
{

	long num = w*h;
	for(int i = 0; i < num ; i ++)
	{
		ret->base[i].r = base[i].r;	
	}
	return true;
}

bool CPIC_RGB::GetG_F(CPIC_RGB * ret)
{

	long num = w*h;
	for(int i = 0; i < num ; i ++)
	{
		ret->base[i].g = base[i].g;	
	}
	return true;
}

bool CPIC_RGB::GetB_F(CPIC_RGB * ret)
{

	long num = w*h;
	for(int i = 0; i < num ; i ++)
	{
		ret->base[i].b = base[i].b;	
	}
	return true;
}

bool CPIC_RGB::SetBitBYTE(int x ,int y,void * target)
{

		if(base == NULL || target == NULL || x != w || y != h)
		{
			return false;
		}
		unsigned char * buf;
		buf = (unsigned char * ) target;
		long pos1,pos2;
		if(w%4 != 0)
		{
			for(long i = 0; i < h ; i ++)
			{
				pos1 = (i)*w;
				pos2 = (i)*Line_Span;
				memcpy(&(base[pos1]),&(buf[pos2]),w*3);
			}
		}
		else
		{
			memcpy(base,buf,w*h*3);
		}
		
		return true;
	

}

bool CPIC_RGB::SetBitBYTEOrg(int x ,int y,void * target)
{
	if(base == NULL || target == NULL || x != w || y != h)
	{
		return false;
	}
	unsigned char * buf;
	buf = (unsigned char * ) target;
	long pos1,pos2;
	for(long i = 0; i < h ; i ++)
	{
		pos1 = (h-1-i)*w;
		pos2 = (i)*Line_Span;
		memcpy(&(base[pos1]),&(buf[pos2]),w*3);
	}
	
	return true;
}

bool CPIC_RGB::GetBitBYTE(int x ,int y,void * target)
{
	if(base == NULL || target == NULL || x != w || y != h)
	{
		return false;
	}
	unsigned char * buf;
	buf = (unsigned char * ) target;
	long pos1,pos2;
	for(long i = 0; i < h ; i ++)
	{
		pos1 = i*w;
		pos2 = (i)*Line_Span;
		memcpy(&(buf[pos2]),&(base[pos1]),w*3);
	}
	return true;
}



bool CPIC_RGB::GetRGB(int x,int y,p_RGB_CELL t)
{
	if(base == NULL || t == NULL || x < 0 || y <0 || x>=w || y >= h)
	{
		return false;
	}
	long pos = w * y + x;
	t->r = base[pos].r;
	t->g = base[pos].g;
	t->b = base[pos].b;
	return true;
}

bool CPIC_RGB::SetRGB(int x,int y,p_RGB_CELL t)
{
	if(base == NULL || t == NULL || x < 0 || y <0 || x>=w || y >= h)
	{
		return false;
	}
	long pos = w * y + x;
	base[pos].r = t->r;
	base[pos].g = t->g;
	base[pos].b = t->b;
	return true;
}



bool CPIC_RGB::init(int x_span,int y_span)
{
	if(x_span<=0 || y_span <=0)
	{
		return false;
	}
	if(base!=NULL)
	{
		if(x_span!= w || y_span != h )
		{
			w = 0; 
			h = 0;
			Line_Span = 0;
			free(base);
			base = (p_RGB_CELL)malloc(sizeof(RGB_CELL)*(x_span)*y_span);
			if(base==NULL)
			{
				return false;
			}
		}
		
		w = x_span;
		h = y_span;
	}
	else
	{
		base = (p_RGB_CELL)malloc(sizeof(RGB_CELL)*(x_span)*y_span);
		if(base==NULL)
		{
			return false;
		}
		w = x_span;
		h = y_span;
	}

	if((w*3 )%4 == 0)
	{
		this->Line_Span = w*3 ;
	}
	else
	{
		this->Line_Span = w*3 + 4 - (w*3 )%4;
	}
	return true;
};


bool CPIC_RGB::CpyPic(CPIC_RGB *ret)
{
	if(ret == NULL)
	{
		return false;
	}
	if(ret->base == NULL || ret->h  != h || ret->w != w)
	{
		return false;
	}
	memcpy(ret->base,base,Line_Span * h);
	return true;
}

bool CPIC_RGB::OnCpyL(CPIC_RGB * ret)
{
	for(long i = 0; i < h ; i ++)
	{
		for(long j = 0; j < w/2; j++)
		{
			long pos1 = w * i + j;
			long pos2 = ret->w * i+ j;

			ret->base[pos2].r = base[pos1].r;
			ret->base[pos2].g = base[pos1].g;
			ret->base[pos2].b = base[pos1].b;
		}
	}
	return true;

}
bool CPIC_RGB::OnCpyR(CPIC_RGB * ret)
{
	for(long i = 0; i < h ; i ++)
	{
		for(long j = w/2; j < w; j++)
		{
			long pos1 = w * i + j;
			long pos2 = ret->w * i+ j-w/2;

			ret->base[pos2].r = base[pos1].r;
			ret->base[pos2].g = base[pos1].g;
			ret->base[pos2].b = base[pos1].b;
		}
	}

	return true;
}
