#ifndef __FILE_SYSTEM_H
#define __FILE_SYSTEM_H

#include <stdlib.h>
#include <string.h>
#include "iostream"
#include "fstream"
#include "time.h"
using namespace std;



///***** ������� *****/
#define BLOCKSIZE       1024        // ���̿��С
#define SIZE            1024000     // ������̿ռ��С
#define END             65535       // FAT�е��ļ�������־
#define FREE            0           // FAT���̿���б�־
#define MAXOPENFILE     10          // ���ͬʱ���ļ�����

#define MAX_TEXT_SIZE  10000


/***** �������ݽṹ *****/

// �ļ����ƿ�
// ���ڼ�¼�ļ��������Ϳ�����Ϣ��ÿ���ļ�����һ��FCB����Ҳ���ļ���Ŀ¼�������
typedef struct FCB {
    char filename[8];           // �ļ���
    char exname[3];             // �ļ���չ��
    unsigned char attribute;    // �ļ������ֶ�
    unsigned short time;        // �ļ�����ʱ��
    unsigned short date;        // �ļ���������
    unsigned short first;       // �ļ���ʼ�̿��
    unsigned long length;       // �ļ����ȣ��ֽ�����
    char free;                  // ��ʾĿ¼���Ƿ�Ϊ�գ���ֵΪ0����ʾ�գ�ֵΪ1����ʾ�ѷ���
} fcb;

// �ļ������
typedef struct FAT {
    unsigned short id;
} fat;

/* �û����ļ��� */
typedef struct USEROPEN {
    char filename[8];           // �ļ���
    char exname[3];             // �ļ���չ��
    unsigned char attribute;    // �ļ������ֶ�
    unsigned short time;        // �ļ�����ʱ��
    unsigned short date;        // �ļ���������
    unsigned short first;       // �ļ���ʼ�̿��
    unsigned long length;       // �ļ����ȣ��������ļ����ֽ�������Ŀ¼�ļ�������Ŀ¼�������
    char free;                  // ��ʾĿ¼���Ƿ�Ϊ�գ���ֵΪ0����ʾ�գ�ֵΪ1����ʾ�ѷ���

    int dirno;                  // ��Ӧ���ļ���Ŀ¼���ڸ�Ŀ¼�ļ��е��̿��
    int diroff;                 // ��Ӧ���ļ���Ŀ¼���ڸ�Ŀ¼�ļ���dirno�̿��е�Ŀ¼�����
    char dir[80];               // ��Ӧ���ļ����ڵ�Ŀ¼��������������ټ���ָ���ļ��Ƿ��Ѿ���
    int count;                  // ��дָ�����ļ��е�λ��
    char fcbstate;              // �Ƿ��޸����ļ���FCB�����ݣ�����޸�����Ϊ1������Ϊ0
    char topenfile;             // ��ʾ���û��򿪱����Ƿ�Ϊ�գ���ֵΪ0����ʾΪ�գ������ʾ�ѱ�ĳ���ļ�ռ��
} useropen;

/* ������ BLOCK0 */
typedef struct BLOCK0 {
    char magic_number[8];       // �ļ�ϵͳ��ħ��
    char information[200];
    unsigned short root;        // ��Ŀ¼�ļ�����ʼ�̿��
    unsigned char *startblock;  // �����������������ʼλ��
} block0;


/***** ȫ�ֱ������� *****/
unsigned char *myvhard;             // ָ��������̵���ʼ��ַ
useropen openfilelist[MAXOPENFILE]; // �û����ļ�������
int currfd;                         // ��¼��ǰ�û����ļ�������±�
unsigned char *startp;              // ��¼�����������������ʼλ��
char *FileName = const_cast<char *>("myfsys.txt");
unsigned char buffer[SIZE];


/***** �������� *****/
// ���
void startSys();                   // �����ļ�ϵͳ
void my_exitsys();                 // �˳��ļ�ϵͳ
void my_format();                  // ���̸�ʽ������
// ��ʫī
void my_mkdir(char *dirname);      // ������Ŀ¼
void my_rmdir(char *dirname);      // ɾ����Ŀ¼
// ������
void my_ls();                      // ��ʾĿ¼�е�����
void my_cd(char *dirname);         // ���ڸ��ĵ�ǰĿ¼
// �ڿ���
int  my_create(char *filename);    // �����ļ�
void my_rm(char *filename);        // ɾ���ļ�
int  my_open(char *filename);      // ���ļ�
int  my_close(int fd);             // �ر��ļ�
// ���
int  my_write(int fd);             // д�ļ�
int  do_write(int fd, char *text, int len, char wstyle);
// ������
int  my_read(int fd);              // ���ļ�
int  do_read(int fd, int len, char *text);


unsigned short getFreeBLOCK();      // ��ȡһ�����еĴ��̿�
int getFreeOpenfilelist();          // ��ȡһ�����е��ļ��򿪱���
int find_father_dir(int fd);        // Ѱ��һ�����ļ��ĸ�Ŀ¼���ļ�


void show_help();
void error(char *command);


/* ����Ƿ��пյ��û��ļ��򿪱��� */
int getFreeOpenfilelist(){
    for(int i=0; i<MAXOPENFILE; i++){
        // �����ǰ�ļ�δ�򿪾ʹ򿪵�ǰ�ļ�
        if(openfilelist[i].topenfile == 0){ 
            openfilelist[i].topenfile = 1;
            return i;
        }
    }
    return -1;
}


/* ��� FAT �Ƿ��п��е��̿� */
unsigned short int getFreeBLOCK(){
    fat* fat1 = (fat*)(myvhard + BLOCKSIZE);
    for(int i=0; i < (int)(SIZE/BLOCKSIZE); i++){
        if(fat1[i].id == FREE){
            return i;
        }
    }
    return END;
}


/* ���ҵ�ǰ�ļ��ĸ�Ŀ¼�ļ� */
int find_father_dir(int fd){
    for(int i=0; i<MAXOPENFILE; i++){
        if(openfilelist[i].first == openfilelist[fd].dirno){
            return i;
        }
    }
    return -1;
}


/* ����ָ�� */
void show_help() {
	printf("������\t\t�������\t\t����˵��\n\n");
	printf("cd\t\tĿ¼��(·����)\t\t�л���ǰĿ¼��ָ��Ŀ¼\n");
	printf("mkdir\t\tĿ¼��\t\t\t�ڵ�ǰĿ¼������Ŀ¼\n");
	printf("rmdir\t\tĿ¼��\t\t\t�ڵ�ǰĿ¼ɾ��ָ��Ŀ¼\n");
	printf("ls\t\t��\t\t\t��ʾ��ǰĿ¼�µ�Ŀ¼���ļ�\n");
	printf("create\t\t�ļ���\t\t\t�ڵ�ǰĿ¼�´���ָ���ļ�\n");
	printf("rm\t\t�ļ���\t\t\t�ڵ�ǰĿ¼��ɾ��ָ���ļ�\n");
	printf("open\t\t�ļ���\t\t\t�ڵ�ǰĿ¼�´�ָ���ļ�\n");
	printf("write\t\t��\t\t\t�ڴ��ļ�״̬�£�д���ļ�\n");
	printf("read\t\t��\t\t\t�ڴ��ļ�״̬�£���ȡ���ļ�\n");
	printf("close\t\t��\t\t\t�ڴ��ļ�״̬�£��رո��ļ�\n");
	printf("exit\t\t��\t\t\t�˳�ϵͳ\n\n");
}


/* ������Ϣ��ʾ */
void error(char *command) {
	printf("%s : ȱ�ٲ���\n", command);
	printf("���� 'help' ���鿴������ʾ.\n");
}


#endif /* __FILE_SYSTEM_H */