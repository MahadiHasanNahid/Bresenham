#include "map.h"

void byteArrayToBitArray(uint8_t* bytes, uint32_t numBytes, uint8_t* bits)
{
    uint32_t word=0;
    uint8_t bit=0;
    for(uint32_t i=0;i<numBytes;i++) {
	if(bytes[i])
	    bits[word]|=1<<bit;
	else
	    bits[word]&=~(1<<bit);
	bit++;
	word+=bit/8;
	bit%=8;
    }
}

void clearMap(Map* map)
{
    uint32_t numBytes=ceil(map->height*map->width/8.f);
    for(uint32_t i=0;i<numBytes;i++)
	map->data[i]=0;
}

void initMap(Map* map, uint16_t height, uint16_t width)
{
    map->height=height;
    map->width=width;
    uint32_t numBytes=ceil(height*width/8.f);
    map->data=malloc(sizeof(uint8_t)*numBytes);
    for(uint32_t i=0;i<numBytes;i++)
	map->data[i]=0;
}

void printMap(Map* map)
{
    uint32_t word=0;
    uint8_t bit=0;
    for(uint32_t y=0;y<map->height;y++) {
	for(uint32_t x=0;x<map->width;x++) {
	    printf("%d",(map->data[word]>>bit)&1);
	    bit++;
	    word+=bit/8;
	    bit%=8;
	}
	printf("\n");
    }
}

uint8_t getVal(uint16_t x, uint16_t y, Map* map)
{
    uint32_t pos=y*map->height+x;
    uint32_t words=pos/8;
    uint8_t bits=pos%8;
    return ((map->data[words])>>bits)&1;
}

void setVal(uint16_t x, uint16_t y, uint8_t val, Map* map) {
    uint32_t pos=y*map->height+x;
    uint32_t words=pos/8;
    uint8_t bits=pos%8;
    if(val)
	map->data[words]|=1<<bits;
    else
	map->data[words]&=~(1<<bits);
}


void drawGrid(Map* map)
{
    glBegin(GL_LINES);
    float cellwidth=2.f/map->width;
    float cellheight=2.f/map->height;
    for(int i=1;i<map->width;i++) {
	float xpos=-1+i*cellwidth;
	glVertex2f(-1,xpos);
	glVertex2f(1,xpos);
    }

    for(int i=1;i<map->height;i++) {
	float ypos=-1+i*cellheight;
	glVertex2f(ypos,-1);
	glVertex2f(ypos,1);
    }
    glEnd();
}

void drawMap(Map* map)
{
    glBegin(GL_QUADS);
    float cellwidth=2.f/map->width;
    float cellheight=2.f/map->height;
    for(uint16_t y=0;y<map->height;y++) {
	for(uint16_t x=0;x<map->width;x++) {
	    if(getVal(x,y,map)) {
		float xl=-1+x*cellwidth;
		float xr=xl+cellwidth;
		float yt=1-y*cellheight;
		float yb=yt-cellheight;
		glVertex2f(xl,yt);
		glVertex2f(xr,yt);
		glVertex2f(xr,yb);
		glVertex2f(xl,yb);
	    }
	}
    }
    glEnd();
}

uint8_t getVal_float(float x, float y, Map* map)
{
    uint16_t xval=floor((x+1)/2*map->width);
    uint16_t yval=floor((y+1)/2*map->height);
    return getVal(xval,yval,map);
}


void setVal_float(float x, float y, uint8_t val, Map* map)
{
    uint16_t xval=(x+1)/2*map->width;
    uint16_t yval=(y+1)/2*map->height;
    setVal(xval,yval,val,map);
}

