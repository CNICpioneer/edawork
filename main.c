#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NETS 100
#define MAX_PINS 100
struct {
	int die_x;
	int die_y;
	int Num_Instances;
	int Num_Nets;	
	}die;
	char line[256];
	int netIndex = 0;
    int pinIndex = 0;
    int netPinMap[MAX_NETS][MAX_PINS] = {0};
    // 结构体表示一个点的坐标
typedef struct {
    int x;
    int y;
} Point;

// 检查两个点是否重合
int isOverlap(Point p1, Point p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}

// 输出点的坐标
void printLayout(Point layout[]) {
    FILE* fput  = fopen("D:\\vscode\\output.txt", "w");
    for (int i = 0; i < die.Num_Instances; ++i) {
        
        fprintf(fput,"(%d, %d)",layout[i].x, layout[i].y);
        printf("(%d, %d) ", layout[i].x, layout[i].y);
    }   
    printf("\n");
    fclose(fput);
    
}

// 检查当前点是否可以放置
int isValid(Point layout[], int current, int x, int y) {
    for (int i = 0; i < current; ++i) {
        if (isOverlap(layout[i], (Point){x, y})) {
            return 0; // 与之前的点重合
        }
    }
    return 1; // 可以放置   
}

// 递归生成所有布局
void generateLayout(Point layout[], int current,int sum,int minsum, Point* minlayout){
    if (current == die.Num_Instances) {
    // 所有点都已放置，输出当前布局
    int length[MAX_NETS][MAX_PINS];
        for (int i = 0; i < MAX_NETS; i++) {
            for (int j = 0; j < MAX_PINS; j++) {  // 创造了一个在范围内的数组，先全都设为0，然后 不断读入 数据，最后输出的时候循环条件就可以不用看是读取的个数，直接if判断如果为0，即没有数据读入就停止输出
                if (netPinMap[i][j + 1] != 0) {      //为什么是j+1，因为下面计算时候我只读了前一个点自动和后一个点计算，但是数组遍历的时候是一直到最后一个pin的，如果继续计算就会和后面 那个空的点计算，这样就算多了东西。               
                    int a = (layout[netPinMap[i][j] - 1].x - layout[netPinMap[i][j + 1] - 1].x);//取出来第一个pin是C1，但是对于数组应该是0，所以-1，j+1指的是下一个pin
                    int b = (layout[netPinMap[i][j] - 1].y - layout[netPinMap[i][j + 1] - 1].y);
                    length[i][j] = a * a + b * b;
                    sum += length[i][j];
                }
            }   
        }
        if (sum < minsum) {                        //更替来得到最小的 sum，同时创造一个minlayout来每次存入更小的那种情况，这样就能保证更小的情况对应上他的布局情况
            minsum = sum;
            for (int i = 0; i < die.Num_Instances; ++i) {
                minlayout[i] = layout[i];
            }
        }       
        return;

    }

    // 尝试在每个格子放置当前点
    for (int x = 0; x < die.Num_Instances-1; ++x) {
        for (int y = 0; y < die.Num_Instances-1; ++y) {
            if (isValid(layout, current, x, y)) {
                // 可以放置当前点
                layout[current].x = x;
                layout[current].y = y;

                // 递归放置下一个点
                generateLayout(layout, current + 1,sum,minsum,minlayout);
            }
        }
    }
}
    int main (){
    FILE* fp = fopen("D:\\vscode\\data.txt", "r");
	while (fgets(line, sizeof(line), fp)) {  //打开文件把并且去每一行读入文件
		
		sscanf(line, "DieSize %d %d ", &die.die_x, &die.die_y);//得到网格规格
		sscanf(line, "NumInstances %d ", &die.Num_Instances); //得到放置点的个数
	 	sscanf(line, "NumNets %d ", &die.Num_Nets);  //得到Net的个数
		if (strstr(line, "Net") != NULL) {//找到Net，进入循环
            sscanf(line, "Net N%d %d", &netIndex, &pinIndex);   //读取每一个Net的标号和后面对应Net Pin的个数

            for (int i = 0; i < pinIndex; i++) {                //根据PIN的个数进行遍历
    
                fgets(line, sizeof(line), fp);                  
                int pinNumber;                              
                sscanf(line, "Pin C%d", &pinNumber);            //读取每个Net里面Pin的连接顺序
                netPinMap[netIndex-1][i] = pinNumber;           //存入 数组，第一个[]代表的是第几个Net，后面[]存入的是Pin的顺序
            }
        }
	}
	fclose(fp);
	printf("die_x=%d die_y=%d Num_Instances=%d NumNets=%d", die.die_x, die.die_y,die.Num_Instances,die.Num_Nets);
	for (int i = 0; i < MAX_NETS; i++) {
        for (int j = 0; j < MAX_PINS; j++) {  // 创造了一个在范围内的数组，先全都设为0，然后 不断读入 数据，最后输出的时候循环条件就可以不用看是读取的个数，直接if判断如果为0，即没有数据读入就停止输出
            if (netPinMap[i][j] != 0) {
                printf("Net N%d Pin C%d\n", i+1, netPinMap[i][j]);
            }
        }
    }
    Point layout[die.Num_Instances] ;
    Point minlayout[die.Num_Instances]; // 存储布局的数组
    generateLayout(layout, 0,0,9999,minlayout); // 从第一个点开始递归生成布局
    printLayout(minlayout);

 return 0 ;
}   