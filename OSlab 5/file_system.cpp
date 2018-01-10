#include "file_system.h"

/***** ������� *****/
// ϵͳ������
int main() {
	char cmd[15][10] = { "mkdir", "rmdir", "ls", "cd", "create", "rm", "open", "close", "write", "read", "exit", "help","bigfile" };
	char command[30], *sp, *len, yesorno;
	int indexOfCmd, i;

	/*************************************************/
	printf("*********** �ļ�ϵͳ **********\n");
	// �����ļ�ϵͳ
	startSys();
	printf("�����ļ�ϵͳ.\n");
	printf("����help����ʾ����ҳ��.\n");

	while (1) {
		printf("%s>", openfilelist[currfd].dir);    //��ǰ�û����ļ���Ŀ¼��
		// gets(command);
		fgets(command,sizeof(command),stdin);
		indexOfCmd = -1;
		if (strcmp(command, "")) {                   // command ��Ϊ��
			sp = strtok(command, " ");              // ���ݿո�ֽ��ַ���
			for (i = 0; i < 15; i++) {               // ʶ���ַ����е�һ��������
				if (strcmp(sp, cmd[i]) == 0) {
					indexOfCmd = i;
					break;
				}
			}
			switch (indexOfCmd) {
			case 0:         // mkdir
				sp = strtok(NULL, " ");         // ����ץȡ command ֮����ַ�
												// ref: http://blog.csdn.net/zhangchaoq/article/details/45852709
				if (sp != NULL)
					my_mkdir(sp);               // ������Ŀ¼������Ŀ¼
				else
					error(const_cast<char *>("mkdir"));
				break;
			case 1:         // rmdir
				sp = strtok(NULL, " ");
				if (sp != NULL)
					my_rmdir(sp);               // ɾ����Ŀ¼������Ŀ¼
				else
					error(const_cast<char *>("rmdir"));
				break;
			case 2:         // ls
				my_ls();                        // ��ʾĿ¼
				break;
			case 3:         // cd
				sp = strtok(NULL, " ");
				if (sp != NULL)
					my_cd(sp);                  // ���ĵ�ǰĿ¼
				else
					error(const_cast<char *>("cd"));
				break;
			case 4:         // create
				sp = strtok(NULL, " ");
				if (sp != NULL)
					my_create(sp);              // �����ļ���Ϊ sp ���ļ�
				else
					error(const_cast<char *>("create"));
				break;
			case 5:         // rm
				sp = strtok(NULL, " ");
				if (sp != NULL)
					my_rm(sp);                  // ɾ���ļ���Ϊ sp ���ļ�
				else
					error(const_cast<char *>("rm"));
				break;
			case 6:         // open
				sp = strtok(NULL, " ");
				if (sp != NULL)
					my_open(sp);                // ���ļ���Ϊ sp ���ļ�
				else
					error(const_cast<char *>("open"));
				break;
			case 7:         // close
				if (openfilelist[currfd].attribute == 1)
					my_close(currfd);           // �ر������ļ�
				else
					cout << "��ǰû�еĴ򿪵��ļ�" << endl;
				break;
			case 8:         // write
				if (openfilelist[currfd].attribute == 1)
					my_write(currfd);           // д�����ݵ��ļ�
				else
					cout << "���ȴ��ļ�,Ȼ����ʹ��wirte����" << endl;
				break;
			case 9:         // read
				if (openfilelist[currfd].attribute == 1)
					my_read(currfd);            //  ���ļ��ж�������
				else
					cout << "���ȴ��ļ�,Ȼ����ʹ��read����" << endl;
				break;
			case 10:        // exit
				my_exitsys();                   // �˳��ļ�ϵͳ
				printf("�˳��ļ�ϵͳ.\n");
				return 0;
				break;
			case 11:        // help
				show_help();                    // �г��ļ�ϵͳ�ṩ�ĸ���ܼ�������ø�ʽ��
				break;
				// case 12:
				//     show_big_file();
				//     break;
			default:
				printf("û�� %s �������\n", sp);
				break;
			}
		}
		else
			printf("\n");
	}
	return 0;
}


/* �����ļ�ϵͳ���� */
/**
* 1. Ѱ��myfsys.txt�ļ�,������ڶ��ҿ�ͷ���ļ�ħ��,�Ͷ�ȡ��myvhard,���򴴽��ļ���д���ʼ����Ϣ
* 2. �����û����ļ���ĵ�һ������, ����Ϊ��Ŀ¼����, Ҳ����Ĭ�ϴ򿪸�Ŀ¼
* 3. ��ʼ��һ��ȫ�ֱ���
*/
void startSys() {

	// ����������̿ռ�
	myvhard = (unsigned char *)malloc(SIZE);

	// ��myfsys�ļ�,���ж��ļ��Ƿ����
	FILE *file;
	if ((file = fopen(FileName, "r")) != NULL) {          // �ļ�����
														  // ����myfsys�ļ����ݵ��û��ռ��е�һ����������
		fread(buffer, SIZE, 1, file);
		// �ж��ļ���ʼ��8���ֽ������Ƿ�Ϊ��10101010�����ļ�ϵͳħ����
		if (memcmp(buffer, "10101010", 8) == 0) {           // ��ħ��
															// �������������е����ݸ��Ƶ��ڴ��е�������̿ռ���
			memcpy(myvhard, buffer, SIZE);
			cout << "myfsys�ļ���ȡ�ɹ�!" << endl;
		}
		else {       // �ļ���ͷ����ħ��
			cout << "myfsys�ļ�ϵͳ�����ڣ����ڿ�ʼ�����ļ�ϵͳ" << endl;
			// ����my_format()��֮ǰ���뵽��������̿ռ���и�ʽ������
			my_format();
			// ����������е����ݱ��浽 myfsys �ļ���
			memcpy(buffer, myvhard, SIZE);
			fwrite(buffer, SIZE, 1, file);
		}
	}
	else {       // �ļ�������
		cout << "myfsys�ļ�ϵͳ�����ڣ����ڿ�ʼ�����ļ�ϵͳ" << endl;
		// ����my_format()�Ԣ������뵽��������̿ռ���и�ʽ������
		my_format();
		// ����������е����ݱ��浽 myfsys �ļ���
		memcpy(buffer, myvhard, SIZE);
		fwrite(buffer, SIZE, 1, file);
	}

	// �ر�  myfsys �ļ�
	fclose(file);


	// ��ʼ���û��ļ���
	// ������0�������Ŀ¼�ļ�ʹ�ã�����д��Ŀ¼�ļ��������Ϣ��
	fcb *root;
	root = (fcb *)(myvhard + 5 * BLOCKSIZE);
	strcpy(openfilelist[0].filename, root->filename);
	strcpy(openfilelist[0].exname, root->exname);
	openfilelist[0].attribute = root->attribute;
	openfilelist[0].time = root->time;
	openfilelist[0].date = root->date;
	openfilelist[0].first = root->first;
	openfilelist[0].length = root->length;
	openfilelist[0].free = root->free;

	// ��Ŀ¼����û���ϼ��ļ�������
	//      dirno����Ŀ¼������ʼ��ţ�Ϊ 5��
	//      diroff��dirno�̿��е�Ŀ¼����ţ� Ϊ 0
	openfilelist[0].dirno = 5;
	openfilelist[0].diroff = 0;
	strcpy(openfilelist[0].dir, "\\root\\");
	openfilelist[0].count = 0;
	openfilelist[0].fcbstate = 0;
	openfilelist[0].topenfile = 1;


	// �� ptrcurdir ָ��ָ����û����ļ�����
	startp = ((block0*)myvhard)->startblock;
	// �����������������ʼλ����Ϊ 0
	currfd = 0;     // ���õȼ��� ptrcurdir ָ��
	return;
}


/* ���̸�ʽ������ */
/**
* 1. ����������(һ���̿�)
* 2. ����FAT1(2���̿�) FAT2(2���̿�)
* 3. ���ø�Ŀ¼�ļ�����������Ŀ¼��.��..(��Ŀ¼�ļ�ռһ���̿�,����Ŀ¼������д������̿������)
*/
void my_format() {

	/* ��������Ϣ */
	// ��������̵�һ������Ϊ������
	block0 *boot = (block0 *)myvhard;
	// ������Ŀ�ʼ��8���ֽ����ļ�ϵͳ��ħ������Ϊ��10101010��
	strcpy(boot->magic_number, "10101010");

	// д���ļ�ϵͳ��������Ϣ
	strcpy(boot->information, "�ļ�ϵͳ,�����䷽ʽ:FAT,���̿ռ����:�����FAT��λʾͼ,Ŀ¼�ṹ:���û��༶Ŀ¼�ṹ.");
	boot->root = 5;
	boot->startblock = myvhard + BLOCKSIZE * 5;   // ��������������̵ĵ�6�鿪ʼ


												  /* �������� FAT ����Ϣ */
												  // ���� FAT1
	fat* fat1 = (fat*)(myvhard + BLOCKSIZE);
	// �����ǰ5���Ѿ�����
	for (int i = 0; i<5; i++) {
		fat1[i].id = END;
	}
	// ����ĺ�995����Ŀ¼�������������У�
	for (int i = 5; i<1000; i++) {
		fat1[i].id = FREE;
	}

	// FAT2 �� FAT1 ��Ϊ����
	fat* fat2 = (fat*)(myvhard + BLOCKSIZE * 3);
	memcpy(fat2, fat1, BLOCKSIZE);


	/* �� FAT2 �󴴽���Ŀ¼�ļ� root */
	// ���������ĵ�1�飨������̵ĵ�6�飩�������Ŀ¼�ļ�
	fat1[5].id = fat2[5].id = END;  //5���̿飨��6�飩����Ŀ¼��ռ����


									/* �ڴ����ϴ������������Ŀ¼���.���͡�..���������ݳ����ļ�����֮ͬ�⣬�����ֶ���ȫ��ͬ */
									// Ŀ¼��"."ָ���Ŀ¼�Լ�
	fcb *root = (fcb*)(myvhard + BLOCKSIZE * 5);
	strcpy(root->filename, ".");
	strcpy(root->exname, "di");
	root->attribute = 0;        // ֻ��

	time_t rawTime = time(NULL);
	struct tm *time = localtime(&rawTime);
	// �˴�Сʱռ 20 λ�� ����ռ 6 λ�� ����ռ 6 λ������������ʾ 0 ~ 60��
	// �˴�������ʵ�� 64 λ�ı����������£��� unsigned short ���͵ĳ���Ϊ 32 λ��
	// 32 λ�ı������������޸����ӵ�λ��Ϊ 5 λ��ʱ�ӵ�λ��Ϊ 5 λ��
	root->time = time->tm_hour * 4096 + time->tm_min * 64 + time->tm_sec;
	root->date = (time->tm_year - 100) * 512 + (time->tm_mon + 1) * 32 + (time->tm_mday);
	root->first = 5;
	root->free = 1;
	root->length = 2 * sizeof(fcb);

	//root2 ָ���Ŀ¼���ĵڶ���fcb,�����׻�Ŀ¼��..,��Ϊ��Ŀ¼��û���ϼ�Ŀ¼,����ָ���Լ�
	fcb* root2 = root + 1;
	memcpy(root2, root, sizeof(fcb));
	strcpy(root2->filename, "..");

	for (int i = 2; i < int(BLOCKSIZE / sizeof(fcb)); i++) {
		root2++;
		strcpy(root2->filename, "");
		root2->free = 0;
	}

	// //д���ļ�����ȥ
	// FILE *fp = fopen(FileName, "w");
	// fwrite(myvhard, SIZE, 1, fp);
	// fclose(fp);
}


/* ���ĵ�ǰĿ¼���� */
/**
* �����ǰ��Ŀ¼�ļ���,��ô��Ҫ�����Ŀ¼�ļ���ȡ��buf��, Ȼ���������ļ����fcb��û��ƥ��dirname��Ŀ¼��(���ұ�����Ŀ¼�ļ�)
* �����,�Ǿ���openfilelist��ȡһ�����ļ�����,�����dirname���Ŀ¼�ļ���fcbд��ȥ,Ȼ���л�currfd=fd
* ���������Ǵ�һ��Ŀ¼
*/
void my_cd(char *dirname) {

	//  �ж���Ҫ�������ļ��ǲ���Ŀ¼�ļ�
	if (openfilelist[currfd].attribute == 1) {    // �������ļ�
		cout << "�����ļ��ﲻ��ʹ��cd, Ҫ���˳��ļ�, ����closeָ��" << endl;
		return;
	}
	else {           // ��Ŀ¼�ļ��ļ�

					 // ����Ŀ¼����ǰĿ¼�����ļ����ݶ�ȡ���ڴ���
		char *buf = (char *)malloc(MAX_TEXT_SIZE);
		openfilelist[currfd].count = 0;
		do_read(currfd, openfilelist[currfd].length, buf);

		// �ڸ�Ŀ¼�ļ��м���µĵ�ǰĿ¼���Ƿ����
		int i = 0;
		fcb* fcbPtr = (fcb*)buf;
		for (; i < int(openfilelist[currfd].length / sizeof(fcb)); i++, fcbPtr++) {
			// length ����Ŀ¼�ļ���Ŀ¼��ĸ���
			if (strcmp(fcbPtr->filename, dirname) == 0 && fcbPtr->attribute == 0) {
				break;
			}
		}

		// ͨ���ļ�����׺�ж������õ����ļ��Ƿ�ΪĿ¼�ļ�
		if (strcmp(fcbPtr->exname, "di") != 0) {          // ����Ŀ¼�ļ�
														  // ��ʾ������Ϣ������
			cout << "������cd��Ŀ¼�ļ�!" << endl;
			return;
		}
		else {           // ��Ŀ¼�ļ�

						 /* �ж���Ҫ cd ��Ŀ¼�� ".", ".." ������Ŀ¼ */
						 // cd "." ����ǰ�ļ��������в���
			if (strcmp(fcbPtr->filename, ".") == 0) {
				return;
			}
			// cd ".."�������ϼ�Ŀ¼
			else if (strcmp(fcbPtr->filename, "..") == 0) {

				// �жϵ�ǰĿ¼�ǹ�Ϊ��Ŀ¼
				if (currfd == 0) {    // �Ǹ�Ŀ¼ֱ�ӷ���
					return;
				}
				else {               // ���Ǹ�Ŀ¼
									 // �رյ�ǰĿ¼�ļ�������
					currfd = my_close(currfd);
					return;
				}
			}
			// cd ���ļ�
			else {

				// Ϊ�½���Ŀ¼�ļ�����һ�����д��ļ�����
				int fd = getFreeOpenfilelist();
				// û�ж�����ļ��򿪱��֣��ļ���ʧ��
				if (fd == -1) {
					return;
				}
				else {

					// ������Ҫ�򿪵���Ŀ¼��ӵ��ļ��򿪱�����
					openfilelist[fd].attribute = fcbPtr->attribute;
					openfilelist[fd].count = 0;
					openfilelist[fd].date = fcbPtr->date;
					openfilelist[fd].time = fcbPtr->time;
					strcpy(openfilelist[fd].filename, fcbPtr->filename);
					strcpy(openfilelist[fd].exname, fcbPtr->exname);
					openfilelist[fd].first = fcbPtr->first;
					openfilelist[fd].free = fcbPtr->free;
					//ǰ����FCB����
					openfilelist[fd].fcbstate = 0;
					openfilelist[fd].length = fcbPtr->length;
					// �޸� openfilelist[fd].dir[fd] = openfilelist[currfd].dir[currfd] + dirname;
					strcpy(openfilelist[fd].dir,
						(char*)(string(openfilelist[currfd].dir) + string(dirname) + string("\\")).c_str());
					openfilelist[fd].topenfile = 1;
					openfilelist[fd].dirno = openfilelist[currfd].first;
					openfilelist[fd].diroff = i;

					// ���õ�ǰĿ¼Ϊ��Ŀ¼
					currfd = fd;
				}
			}
		}
	}
}


/* ������Ŀ¼���� */
void my_mkdir(char *dirname) {

	//�ж�dirname�Ƿ�Ϸ�
	char* fname = strtok(dirname, ".");
	char* exname = strtok(NULL, ".");
	if (exname) {
		cout << "�����������׺��!" << endl;
		return;
	}

	// ����ǰ�ļ����ݶ��뵽�ڴ�
	char text[MAX_TEXT_SIZE];
	openfilelist[currfd].count = 0;
	int fileLen = do_read(currfd, openfilelist[currfd].length, text);

	// ��鵱ǰĿ¼���½�Ŀ¼�ļ��Ƿ�����
	fcb *fcbPtr = (fcb*)text;       //text������ݾ���һ����fcb
	for (int i = 0; i < (int)(fileLen / sizeof(fcb)); i++) {
		if (strcmp(dirname, fcbPtr[i].filename) == 0 && fcbPtr->attribute == 0) {  // ����
																				   // ��ʾ����������Ϣ������
			cout << "Ŀ¼���Ѿ�����!" << endl;
			return;
		}
	}

	/* Ϊ�½���Ŀ¼�ļ�����һ�����д��ļ����� */
	//�ڴ��ļ�������һ�����ļ�����
	int fd = getFreeOpenfilelist();
	if (fd == -1) {       // �ļ��򿪱�������
		cout << "���ļ�����ȫ����ռ��" << endl;
		return;
	}
	// ��� FAT �Ƿ��п��е��̿鲢Ϊ�µ��½�Ŀ¼�ļ�����һ���̿�
	unsigned short int blockNum = getFreeBLOCK();
	if (blockNum == END) {
		// �ͷŵ�ǰ��Ŀ¼������û��ļ��򿪱���
		openfilelist[fd].topenfile = 0;

		// ���ش�����Ϣ
		cout << "�̿��Ѿ�����" << endl;
		return;
	}

	/* �ڵ�ǰĿ¼��׷��һ���µ�Ŀ¼�� */
	// �򿪵�ǰĿ¼
	fat *fat1 = (fat *)(myvhard + BLOCKSIZE);
	fat *fat2 = (fat *)(myvhard + BLOCKSIZE * 3);
	fat1[blockNum].id = END;
	fat2[blockNum].id = END;
	// �޸ĵ�ǰĿ¼�ļ��ĳ�����Ϣ
	int i = 0;
	for (; i < (int)(fileLen / sizeof(fcb)); i++) {
		if (fcbPtr[i].free == 0) {
			break;
		}
	}
	openfilelist[currfd].count = i * sizeof(fcb);
	// ����ǰĿ¼�ļ����û����ļ������е� fcbstate ��Ϊ 1
	openfilelist[currfd].fcbstate = 1;


	// ׼�����½�Ŀ¼�ļ��� FCB ������
	fcb* fcbtmp = new fcb;
	fcbtmp->attribute = 0;
	time_t rawtime = time(NULL);
	struct tm* time = localtime(&rawtime);
	fcbtmp->date = (time->tm_year - 100) * 512 + (time->tm_mon + 1) * 32 + (time->tm_mday);
	fcbtmp->time = (time->tm_hour) * 4096 + (time->tm_min) * 64 + (time->tm_sec);
	strcpy(fcbtmp->filename, dirname);
	strcpy(fcbtmp->exname, "di");
	fcbtmp->first = blockNum;
	fcbtmp->length = 2 * sizeof(fcb);
	fcbtmp->free = 1;
	// �Ը���д��ʽ���� do_write() ������д����Ӧ�Ŀ�Ŀ¼����
	do_write(currfd, (char *)fcbtmp, sizeof(fcb), 1);


	// ���½�Ŀ¼�ļ������䵽�Ĵ��̿��н������������Ŀ¼�.���͡�..��Ŀ¼��
	// ���ô��ļ�����
	openfilelist[fd].attribute = 0;
	openfilelist[fd].count = 0;
	openfilelist[fd].date = fcbtmp->date;
	openfilelist[fd].time = fcbtmp->time;
	openfilelist[fd].dirno = openfilelist[currfd].first;
	openfilelist[fd].diroff = i;
	strcpy(openfilelist[fd].exname, "di");
	strcpy(openfilelist[fd].filename, dirname);
	openfilelist[fd].fcbstate = 0;
	openfilelist[fd].first = fcbtmp->first;
	openfilelist[fd].free = fcbtmp->free;
	openfilelist[fd].length = fcbtmp->length;
	openfilelist[fd].topenfile = 1;
	// �޸� openfilelist[fd].dir[fd] = openfilelist[currfd].dir[currfd] + dirname;
	strcpy(openfilelist[fd].dir, (char*)(string(openfilelist[currfd].dir) + string(dirname) + string("\\")).c_str());

	// �ڶ�Ӧ���̿���������������Ŀ¼.��..
	// ���û��ռ���׼�� "." Ŀ¼�ļ�������
	fcbtmp->attribute = 0;
	fcbtmp->date = fcbtmp->date;
	fcbtmp->time = fcbtmp->time;
	strcpy(fcbtmp->filename, ".");
	strcpy(fcbtmp->exname, "di");
	fcbtmp->first = blockNum;
	fcbtmp->length = 2 * sizeof(fcb);
	// �Խض�д���߸���д��ʽ���� do_write()����д��֮ǰ���䵽�Ŀ��д��̿��У�
	do_write(fd, (char*)fcbtmp, sizeof(fcb), 1);

	// ͬ���� ".." Ŀ¼�ļ�������
	fcb *fcbtmp2 = new fcb;
	memcpy(fcbtmp2, fcbtmp, sizeof(fcb));
	strcpy(fcbtmp2->filename, "..");
	fcbtmp2->first = openfilelist[currfd].first;
	fcbtmp2->length = openfilelist[currfd].length;
	fcbtmp2->date = openfilelist[currfd].date;
	fcbtmp2->time = openfilelist[currfd].time;
	do_write(fd, (char*)fcbtmp2, sizeof(fcb), 1);


	my_close(fd);
	// ?
	// ���±�currfdĿ¼�ļ���fcb
	fcbPtr = (fcb *)text;
	fcbPtr->length = openfilelist[currfd].length;
	openfilelist[currfd].count = 0;
	do_write(currfd, (char*)fcbPtr, sizeof(fcb), 1);
	openfilelist[currfd].fcbstate = 1;
	delete fcbtmp;
	delete fcbtmp2;
}


/* ɾ����Ŀ¼���� */
void my_rmdir(char *dirname) {

	char* fname = strtok(dirname, ".");
	char* exname = strtok(NULL, ".");
	//������ɾ��.��..����������Ŀ¼�ļ�
	if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0) {
		cout << "����ɾ��" << dirname << "�������Ŀ¼��" << endl;
		return;
	}
	if (exname) {
		cout << "ɾ��Ŀ¼�ļ����������׺��!" << endl;
		return;
	}

	// ��ȡ��ǰĿ¼�ļ����ݵ��ڴ�
	char buf[MAX_TEXT_SIZE];
	openfilelist[currfd].count = 0;
	do_read(currfd, openfilelist[currfd].length, buf);


	// ��鵱ǰĿ¼����ɾ��Ŀ¼�ļ��Ƿ����
	int i;
	fcb* fcbPtr = (fcb*)buf;
	for (i = 0; i < int(openfilelist[currfd].length / sizeof(fcb)); i++, fcbPtr++) {
		if (strcmp(fcbPtr->filename, fname) == 0 && fcbPtr->attribute == 0) {
			break;
		}
	}
	// �������򷵻ش�����Ϣ
	if (i == int(openfilelist[currfd].length / sizeof(fcb))) {
		cout << "û�����Ŀ¼�ļ�" << endl;
		return;
	}


	// �����ɾ��Ŀ¼�ļ��Ƿ�Ϊ�գ����ˡ�.���͡�..����û��������Ŀ¼���ļ���
	// ������Ŀ¼���м�¼���ļ��������ж�
	if (fcbPtr->length > 2 * sizeof(fcb)) {
		cout << "����������Ŀ¼�µ������ļ�,��ɾ��Ŀ¼�ļ�" << endl;
		return;
	}


	// ���ո�Ŀ¼�ļ���ռ�ݵĴ��̿飬�޸� FAT
	int blockNum = fcbPtr->first;
	fat* fat1 = (fat*)(myvhard + BLOCKSIZE);
	int next = 0;
	while (1) {
		next = fat1[blockNum].id;
		fat1[blockNum].id = END;
		if (next != END) {
			blockNum = next;
		}
		else {
			break;
		}
	}
	// ���� FAT2
	fat1 = (fat*)(myvhard + BLOCKSIZE);
	fat* fat2 = (fat*)(myvhard + BLOCKSIZE * 3);
	memcpy(fat2, fat1, sizeof(fat));


	// �ӵ�ǰĿ¼�ļ�����ո�Ŀ¼�ļ���Ŀ¼��
	fcbPtr->date = 0;
	fcbPtr->time = 0;
	fcbPtr->exname[0] = '\0';
	fcbPtr->filename[0] = '\0';
	fcbPtr->first = 0;
	fcbPtr->free = 0;
	fcbPtr->length = 0;
	// д��������ȥ, ���� FCB ����Ϊ��
	openfilelist[currfd].count = i * sizeof(fcb);
	do_write(currfd, (char*)fcbPtr, sizeof(fcb), 1);
	openfilelist[currfd].length -= sizeof(fcb);


	// �޸ĵ�ǰĿ¼�ļ����û��򿪱����еĳ�����Ϣ
	fcbPtr = (fcb*)buf;
	fcbPtr->length = openfilelist[currfd].length;
	openfilelist[currfd].count = 0;
	do_write(currfd, (char*)fcbPtr, sizeof(fcb), 1);
	openfilelist[currfd].fcbstate = 1;
}


/* ��ʾĿ¼���� */
void my_ls() {

	// ȷ�����ʵ���Ŀ¼�ļ�
	if (openfilelist[currfd].attribute == 1) {
		cout << "�������ļ��ﲻ��ʹ��ls" << endl;
		return;
	}


	// ���� do_read()������ǰĿ¼�ļ����ݵ��ڴ�
	char buf[MAX_TEXT_SIZE];
	openfilelist[currfd].count = 0;
	do_read(currfd, openfilelist[currfd].length, buf);


	// ��������Ŀ¼�ļ�����Ϣ����һ���ĸ�ʽ��ʾ����Ļ��	
	fcb* fcbPtr = (fcb*)buf;
	for (int i = 0; i < (int)(openfilelist[currfd].length / sizeof(fcb)); i++) {
		if (fcbPtr->free == 1) {
			// Ŀ¼�ļ�
			// ���ռ23λ,�·�ռ4λ,����ռ5λ
			// Сʱռ20λ,����ռ6λ,��ռ6λ
			if (fcbPtr->attribute == 0) {
				printf("%s\\\t<DIR>\t%d/%d/%d\t%d:%d:%d\n",
					fcbPtr->filename,
					(fcbPtr->date >> 9) + 2000,
					(fcbPtr->date >> 5) & 0x000f,
					(fcbPtr->date) & 0x001f,
					(fcbPtr->time >> 12),
					(fcbPtr->time >> 6) & 0x003f,
					(fcbPtr->time) & 0x003f);
			}
			// �����ļ�
			else {
				// length - 2 ����Ϊĩβ��/n��/0�����ַ�
				unsigned int length = fcbPtr->length;
				if (length != 0)
					length -= 2;
				printf("%s.%s\t%dB\t%d/%d/%d\t%02d:%02d:%02d\n",
					fcbPtr->filename,
					fcbPtr->exname,
					length,
					(fcbPtr->date >> 9) + 2000,
					(fcbPtr->date >> 5) & 0x000f,
					(fcbPtr->date) & 0x001f,
					(fcbPtr->time >> 12),
					(fcbPtr->time >> 6) & 0x003f,
					(fcbPtr->time) & 0x003f);
			}
		}
		fcbPtr++;
	}
}


/* �����ļ����� */
int my_create(char *filename) {
	
	char* fname = strtok(filename, ".");
	char* exname = strtok(NULL, ".");
	if (strcmp(fname, "") == 0) {
		cout << "�������ļ���!" << endl;
		return -1;
	}
	if (!exname) {
		cout << "�������׺��!" << endl;
		return -1;
	}
	if (openfilelist[currfd].attribute == 1) {
		cout << "�����ļ��²�����ʹ��create" << endl;
		return -1;
	}


	// �����ø�Ŀ¼�ļ����ݵ��ڴ�
	openfilelist[currfd].count = 0;
	char buf[MAX_TEXT_SIZE];
	do_read(currfd, openfilelist[currfd].length, buf);


	// ����Ŀ¼�����ļ��Ƿ�����
	int i;
	fcb* fcbPtr = (fcb*)(buf);
	for (i = 0; i < int(openfilelist[currfd].length / sizeof(fcb)); i++, fcbPtr++) {
		if (strcmp(fcbPtr->filename, filename) == 0 && strcmp(fcbPtr->exname, exname) == 0) {
			cout << "�ļ����Ѿ�����!" << endl;
			return -1;
		}
	}


	// Ϊ���ļ�����һ���̿�
	int blockNum = getFreeBLOCK();
	if (blockNum == -1) {
		return -1;
	}


	// Ϊ���ļ�Ѱ��һ�����е�Ŀ¼��
	fcbPtr = (fcb*)(buf);
	fcb* debug = (fcb*)(buf);
	for (i = 0; i < int(openfilelist[currfd].length / sizeof(fcb)); i++, fcbPtr++) {
		if (fcbPtr->free == 0)break;
	}


	// ׼�����ļ��� FCB ������
	// �ļ�������Ϊ�����ļ�������Ϊ 0
	fat* fat1 = (fat*)(myvhard + BLOCKSIZE);
	fat1[blockNum].id = END;
	fat* fat2 = (fat*)(myvhard + BLOCKSIZE * 3);
	memcmp(fat2, fat1, BLOCKSIZE * 2);
	// �� FCB ��д��Ϣ
	strcpy(fcbPtr->filename, filename);
	strcpy(fcbPtr->exname, exname);
	time_t rawtime = time(NULL);
	struct tm* time = localtime(&rawtime);
	fcbPtr->date = (time->tm_year - 100) * 512 + (time->tm_mon + 1) * 32 + (time->tm_mday);
	fcbPtr->time = (time->tm_hour) * 4096 + (time->tm_min) * 64 + (time->tm_sec);
	fcbPtr->first = blockNum;
	fcbPtr->free = 1;
	fcbPtr->length = 0;
	fcbPtr->attribute = 1;
	openfilelist[currfd].count = i * sizeof(fcb);
	do_write(currfd, (char *)fcbPtr, sizeof(fcb), 1);


	// �޸ĵ�ǰĿ¼�ļ���.Ŀ¼��ĳ���
	fcbPtr = (fcb*)buf;
	fcbPtr->length = openfilelist[currfd].length;
	openfilelist[currfd].count = 0;
	do_write(currfd, (char*)fcbPtr, sizeof(fcb), 1);
	openfilelist[currfd].fcbstate = 1;
}


/* ɾ���ļ����� */
void my_rm(char *filename) {

	char* fname = strtok(filename, ".");
	char* exname = strtok(NULL, ".");
	if (!exname) {
		cout << "�������׺��!" << endl;
		return;
	}
	if (strcmp(exname, "di") == 0) {
		cout << "����ɾ��Ŀ¼��" << endl;
		return;
	}


	// �����ø�Ŀ¼�ļ����ݵ��ڴ�
	char buf[MAX_TEXT_SIZE];
	openfilelist[currfd].count = 0;
	do_read(currfd, openfilelist[currfd].length, buf);

	// ����Ŀ¼����ɾ���ļ��Ƿ����
	int i;
	fcb* fcbPtr = (fcb*)buf;
	for (i = 0; i < int(openfilelist[currfd].length / sizeof(fcb)); i++, fcbPtr++) {
		if (strcmp(fcbPtr->filename, fname) == 0 && strcmp(fcbPtr->exname, exname) == 0) {
			break;
		}
	}
	// ���������򷵻أ�����ʾ������Ϣ
	if (i == int(openfilelist[currfd].length / sizeof(fcb))) {
		cout << "û������ļ�" << endl;
		return;
	}

	// *
	// ���ո��ļ���ռ�ݵĴ��̿飬�޸� FAT
	int blockNum = fcbPtr->first;
	fat* fat1 = (fat*)(myvhard + BLOCKSIZE);
	int next = 0;
	while (1) {
		next = fat1[blockNum].id;
		fat1[blockNum].id = FREE;
		if (next != END) {
			blockNum = next;
		}
		else {
			break;
		}
	}
	// ���� FAT2
	fat1 = (fat*)(myvhard + BLOCKSIZE);
	fat* fat2 = (fat*)(myvhard + BLOCKSIZE * 3);
	memcpy(fat2, fat1, sizeof(fat));


	// ���ļ��ĸ�Ŀ¼�ļ�����ո��ļ���Ŀ¼��� free �ֶ���Ϊ 0
	fcbPtr->date = 0;
	fcbPtr->time = 0;
	fcbPtr->exname[0] = '\0';
	fcbPtr->filename[0] = '\0';
	fcbPtr->first = 0;
	fcbPtr->free = 0;
	fcbPtr->length = 0;
	//д��������ȥ, ���� FCB ����Ϊ��
	openfilelist[currfd].count = i * sizeof(fcb);
	do_write(currfd, (char*)fcbPtr, sizeof(fcb), 1);
	openfilelist[currfd].length -= sizeof(fcb);


	// �޸ĸø�Ŀ¼�ļ����û����ļ������еĳ�����Ϣ
	fcbPtr = (fcb*)buf;
	fcbPtr->length = openfilelist[currfd].length;
	openfilelist[currfd].count = 0;
	do_write(currfd, (char*)fcbPtr, sizeof(fcb), 1);
	openfilelist[currfd].fcbstate = 1;
}


/* ���ļ����� */
int my_open(char *filename) {

	// �Ƿ�Ϊ�����ļ�
	char *fname = strtok(filename, ".");
	char *exname = strtok(NULL, ".");
	if (!exname) {
		cout << "�������׺��" << endl;
		return -1;
	}


	// ����Ŀ¼�ļ������ݵ��ڴ�
	char buf[MAX_TEXT_SIZE];
	openfilelist[currfd].count = 0;
	do_read(currfd, openfilelist[currfd].length, buf);


	// ����Ŀ¼�������ļ��Ƿ���� 
	int i;
	fcb* fcbPtr = (fcb*)buf;
	for (i = 0; i < int(openfilelist[currfd].length / sizeof(fcb)); i++, fcbPtr++) {
		if (strcmp(fcbPtr->filename, fname) == 0 && strcmp(fcbPtr->exname, exname) == 0 && fcbPtr->attribute == 1) {
			break;
		}
	}
	if (i == int(openfilelist[currfd].length / sizeof(fcb))) {
		cout << "�����ڴ��ļ�!" << endl;
		return -1;
	}


	// Ϊ�����ļ�����һ���ձ���
	int fd = getFreeOpenfilelist();
	if (fd == -1) {
		cout << "�û����ļ����Ѿ�����" << endl;
		return -1;
	}

	
	// Ϊ���ļ���д�հ��û����ļ���������ݣ���дָ����Ϊ 0
	openfilelist[fd].attribute = 1;
	openfilelist[fd].count = 0;
	openfilelist[fd].date = fcbPtr->date;
	openfilelist[fd].time = fcbPtr->time;
	strcpy(openfilelist[fd].exname, exname);
	strcpy(openfilelist[fd].filename, fname);
	openfilelist[fd].length = fcbPtr->length;
	openfilelist[fd].first = fcbPtr->first;
	strcpy(openfilelist[fd].dir, (string(openfilelist[currfd].dir) + string(filename)).c_str());
	openfilelist[fd].dirno = openfilelist[currfd].first;
	openfilelist[fd].diroff = i;
	openfilelist[fd].free = 1;
	openfilelist[fd].topenfile = 1;
	openfilelist[fd].fcbstate = 0;


	// �����ļ������䵽�Ŀհ��û����ļ��������ţ������±꣩��Ϊ�ļ������� fd ����
	currfd = fd;
	return 1;
}


/* �ر��ļ����� */
int my_close(int fd) {

	// ��� fd ����Ч�ԣ�fd ���ܳ����û����ļ����������������±꣩
	if (fd > MAXOPENFILE || fd < 0) {
		cout << "������������ļ�" << endl;
		return -1;
	}
	else {
		// �жϸ�Ŀ¼�ļ��Ƿ����, �����ڱ���
		int fatherFd = find_father_dir(fd);
		if (fatherFd == -1) {
			cout << "��Ŀ¼����!" << endl;
			return -1;
		}

		//fcb���޸���, Ҫд��ȥ
		//�Ǿ�Ҫ�ȰѸ�Ŀ¼�ļ��Ӵ����ж�ȡ��buf��,Ȼ��Ѹ��º��fcb����д��buf��, Ȼ���ٴ�bufд��������
		//��˵һ��,д�ص����̵�ʱ��,ֻҪ���α�countָ������ļ��Ķ�Ӧfcb��λ��,Ȼ��д��fcb�ͺ���
		//����ֻҪдһ��fcb��С�����ݾ�����

		if (openfilelist[fd].fcbstate == 1) {
			// ��ȡ��Ŀ¼�ļ����ڴ���
			char buf[MAX_TEXT_SIZE];
			do_read(fatherFd, openfilelist[fatherFd].length, buf);
			// ���� FCB 
			fcb* fcbPtr = (fcb *)(buf + sizeof(fcb) * openfilelist[fd].diroff);
			strcpy(fcbPtr->exname, openfilelist[fd].exname);
			strcpy(fcbPtr->filename, openfilelist[fd].filename);
			fcbPtr->first = openfilelist[fd].first;
			fcbPtr->free = openfilelist[fd].free;
			fcbPtr->length = openfilelist[fd].length;
			openfilelist[fatherFd].count = 0;
			fcbPtr->time = openfilelist[fd].time;
			fcbPtr->date = openfilelist[fd].date;
			fcbPtr->attribute = openfilelist[fd].attribute;
			openfilelist[fatherFd].count = openfilelist[fd].diroff * sizeof(fcb);
			
			// ���½����� FCB д�뵽 ��Ŀ¼�ļ���Ӧ���̿���
			do_write(fatherFd, (char*)fcbPtr, sizeof(fcb), 1);
		}

		// ���ո��ļ�ռ�ݵ��û����ļ�����������ղ�����
		memset(&openfilelist[fd], 0, sizeof(USEROPEN));

		// �޸ĵ�ǰ�±�
		currfd = fatherFd;
		return fatherFd;
	}

}


/* д�ļ����� */
int my_write(int fd) {

	// ��� fd ����Ч�ԣ�fd ���ܳ����û����ļ����������������±꣩
	if (fd < 0 || fd >= MAXOPENFILE) {
		cout << "�ļ�������" << endl;
		return -1;
	}

	
	// ��ʾ���ȴ��û�����д��ʽ����1���ض�д�� 2������д�� 3��׷��д��
	int wstyle;
	while (1) {
		cout << "����: 0=�ض�д, 1=����д, 2=׷��д" << endl;
		cin >> wstyle;
		if (wstyle > 2 || wstyle < 0) {
			cout << "ָ�����!" << endl;
		}
		else {
			break;
		}
	}
	char text[MAX_TEXT_SIZE] = "\0";
	char textTmp[MAX_TEXT_SIZE] = "\0";


	// ��ʾ�û���������������ͨ�� CTR+Z �����������趨�ļ���������
	// �û��ɷֶ������д�����ݣ�ÿ���ûس�����
	cout << "�������ļ�����, �Ի���+ctrl+z Ϊ�ļ���β" << endl;
	getchar();
	// while (gets(textTmp)) {
	while (fgets(textTmp,sizeof(textTmp),stdin)) {
		textTmp[strlen(textTmp)] = '\n';
		strcat(text, textTmp);
	}
	text[strlen(text)] = '\0';
	

	// ���� do_write()������ͨ�����̼��������д���ļ���
	do_write(fd, text, strlen(text) + 1, wstyle);  	//+1����ΪҪ�ѽ�β��\0Ҳд��ȥ
	
	
	// �޸Ĵ��ļ������е��ļ�������Ϣ������ fcbstate �� 1
	openfilelist[fd].fcbstate = 1;

	return 1;
}


/* ʵ��д�ļ����� */
int do_write(int fd, char *text, int len, char wstyle) {

	//�̿��
	int blockNum = openfilelist[fd].first;
	fat *fatPtr = (fat*)(myvhard + BLOCKSIZE) + blockNum;
	//����д�뷽ʽԤ����
	// 0�ض�д,ֱ�Ӵ�ͷ��ʼд,ƫ��������0��,���ҳ��ȱ��0
	fat* fat1 = (fat*)(myvhard + BLOCKSIZE);
	if (wstyle == 0) {
		openfilelist[fd].count = 0;
		openfilelist[fd].length = 0;
	}
	//1,����д, ����������ļ�,��ôҪ����ɾ���ļ�ĩβ��\0���ܼ�������д
	else if (wstyle == 1) {
		if (openfilelist[fd].attribute == 1 && openfilelist[fd].length != 0) {
			openfilelist[fd].count -= 1;
		}
	}
	//2׷��д,�Ͱ��α�ָ��ĩβ
	else if (wstyle == 2) {
		if (openfilelist[fd].attribute == 0) {
			openfilelist[fd].count = openfilelist[fd].length;
		}
		//ͬ��,����������ļ�Ҫɾ��ĩβ��\0
		else if (openfilelist[fd].attribute == 1 && openfilelist[fd].length != 0) {
			openfilelist[fd].count = openfilelist[fd].length - 1;
		}
	}


	int off = openfilelist[fd].count;										//??????????

																			//���off > BLOCKSIZE,Ҳ�����α�����ָ��Ĳ����ļ��еĵ�һ���̿�,��ô��Ҫ�ҵ��Ǹ��̿�
																			//����,����α�ܴ�,������Ѱ�Ҷ�Ӧ�̿��ʱ����û���Ǹ��̿�,��ô��ȱ�ٵ��̿�ȫ������
	while (off >= BLOCKSIZE) {
		blockNum = fatPtr->id;
		if (blockNum == END) {
			blockNum = getFreeBLOCK();
			if (blockNum == END) {
				cout << "�̿鲻��" << endl;
				return -1;
			}
			else {
				//�޸���fat, ����Ҫ�޸�fat2, �����ж��޸���û��fat, ֱ�����Ը���fat1��fat2����
				fatPtr->id = blockNum;
				fatPtr = (fat*)(myvhard + BLOCKSIZE + blockNum);
				fatPtr->id = END;
			}
		}
		fatPtr = (fat*)(myvhard + BLOCKSIZE) + blockNum;
		off -= BLOCKSIZE;
	}

	unsigned char *buf = (unsigned char*)malloc(BLOCKSIZE * sizeof(unsigned char));
	if (buf == NULL) {
		cout << "�����ڴ�ռ�ʧ��!";
		return -1;
	}


	fcb *dBlock = (fcb *)(myvhard + BLOCKSIZE * blockNum);
	fcb *dFcb = (fcb *)(text);
	unsigned char *blockPtr = (unsigned char *)(myvhard + BLOCKSIZE * blockNum);					//�̿�ָ��
	int lenTmp = 0;
	char *textPtr = text;
	fcb *dFcbBuf = (fcb *)(buf);
	//�ڶ���ѭ��,��ȡ�̿����ݵ�buf, ��text����д��buf, Ȼ���ٴ�bufд�뵽�̿�
	while (len > lenTmp) {
		//�̿����ݶ�ȡ��buf��
		memcpy(buf, blockPtr, BLOCKSIZE);
		//��text����д��buf����ȥ
		for (; off < BLOCKSIZE; off++) {
			*(buf + off) = *textPtr;
			textPtr++;
			lenTmp++;
			if (len == lenTmp) {
				break;
			}
		}
		//��buf����д���̿�����ȥ
		memcpy(blockPtr, buf, BLOCKSIZE);
		//���off==BLCOKSIZE,��ζ��bufд����, ���len != lebTmp ��ζ�����ݻ�ûд��, ��ô��Ҫ��������ļ�����û��ʣ���̿�
		//û��ʣ���̿�,�Ǿ�Ҫ�����µ��̿���
		if (off == BLOCKSIZE && len != lenTmp) {
			off = 0;
			blockNum = fatPtr->id;
			if (blockNum == END) {
				blockNum = getFreeBLOCK();
				if (blockNum == END) {
					cout << "�̿�������" << endl;
					return -1;
				}
				else {
					blockPtr = (unsigned char *)(myvhard + BLOCKSIZE * blockNum);
					fatPtr->id = blockNum;
					fatPtr = (fat *)(myvhard + BLOCKSIZE) + blockNum;
					fatPtr->id = END;
				}
			}
			else {
				blockPtr = (unsigned char *)(myvhard + BLOCKSIZE * blockNum);
				fatPtr = (fat *)(myvhard + BLOCKSIZE) + blockNum;
			}
		}
	}
	openfilelist[fd].count += len;
	//����дָ�����ԭ���ļ��ĳ��ȣ����޸��ļ��ĳ���
	if (openfilelist[fd].count > openfilelist[fd].length)
		openfilelist[fd].length = openfilelist[fd].count;
	free(buf);
	//���ԭ���ļ�ռ�����̿�,�����޸����ļ�,���ռ�õ��̿������,�Ǿ�Ҫ�Ѻ���ռ�õ��̿�ȫ���ͷŵ�
	int i = blockNum;
	while (1) {
		//������fat����һ��fat����end,��ô�����ͷŵ���,һ·�ͷ���ȥ
		if (fat1[i].id != END) {
			int next = fat1[i].id;
			fat1[i].id = FREE;
			i = next;
		}
		else {
			break;
		}
	}
	//�����������ֲ���,��ѱ��ļ������һ���̿�Ҳ���free,����Ҫ�����������ó�END
	fat1[blockNum].id = END;
	//����fat2
	memcpy((fat*)(myvhard + BLOCKSIZE * 3), (fat*)(myvhard + BLOCKSIZE), sizeof(fcb));
	return len;

}


/* ���ļ����� */
int my_read(int fd) {

	// ��� fd ����Ч��
	if (fd >= MAXOPENFILE || fd < 0) {
		cout << "�ļ�������" << endl;
		return -1;
	}


	openfilelist[fd].count = 0;
	char text[MAX_TEXT_SIZE] = "\0";		// ���������û����ļ��ж������ļ�����


	// ��ָ���ļ��е� len �ֽ����ݶ����� text[]��
	do_read(fd, openfilelist[fd].length, text);
	

	// �� text[]�е�������ʾ����Ļ��
	cout << text;

	return 1;
}


/* ʵ�ʶ��ļ����� */
int do_read(int fd, int len, char *text) {

	//lenTmp ���ڼ�¼Ҫ���ȡ�ĳ���,һ�᷵��ʵ�ʶ�ȡ�ĳ���
	int lenTmp = len;

	unsigned char* buf = (unsigned char*)malloc(1024);
	if (buf == NULL) {
		cout << "do_read�����ڴ�ռ�ʧ��" << endl;
		return -1;
	}

	int off = openfilelist[fd].count;
	//��ǰfd��Ӧ����ʼ�̿��, �����ɵ�ǰ�̿��
	int blockNum = openfilelist[fd].first;
	//ptrfat ��ǰ�̿��Ӧ��fat
	fat* fatPtr = (fat *)(myvhard + BLOCKSIZE) + blockNum;
	while (off >= BLOCKSIZE) {
		off -= BLOCKSIZE;
		blockNum = fatPtr->id;
		if (blockNum == END) {
			cout << "do_readѰ�ҵĿ鲻����" << endl;
			return -1;
		}
		fatPtr = (fat*)(myvhard + BLOCKSIZE) + blockNum;
	}
	//��ǰ�̿�Ŷ�Ӧ���̿�
	unsigned char* blockPtr = myvhard + BLOCKSIZE * blockNum;
	//���ļ����ݶ���buf
	memcpy(buf, blockPtr, BLOCKSIZE);
	char *textPtr = text;
	fcb* debug = (fcb*)text;
	while (len > 0) {
		//һ���̿���ܷŵ���
		if (BLOCKSIZE - off > len) {
			memcpy(textPtr, buf + off, len);
			textPtr += len;
			off += len;
			openfilelist[fd].count += len;
			len = 0;
		}
		else {
			memcpy(textPtr, buf + off, BLOCKSIZE - off);
			textPtr += BLOCKSIZE - off;
			off = 0;
			len -= BLOCKSIZE - off;
			//Ѱ����һ����
			blockNum = fatPtr->id;
			if (blockNum == END) {
				cout << "len���ȹ���! �������ļ���С!" << endl;
				break;
			}
			fatPtr = (fat*)(myvhard + BLOCKSIZE) + blockNum;
			blockPtr = myvhard + BLOCKSIZE * blockNum;
			memcpy(buf, blockPtr, BLOCKSIZE);
		}
	}
	free(buf);
	return lenTmp - len;
}

/* �˳��ļ�ϵͳ���� */
void my_exitsys() {
	//��currfd=0��ʱ��,Ҳ���Ǹ��ڵ�, ���ǲ��ø��µ�
	//��Ϊ����֮ǰ���κ�Ŀ¼��mkdir����createʱ,�Ѿ���length�ı仯д��Ŀ¼�ļ���,���ֽ�.��Ŀ¼������
	//���κ�Ŀ¼�ļ���'.'Ŀ¼���ʵʱ���µ�,�������ĸ�Ŀ¼�ļ���û�б�����,������Ҫһ����close
	//���Ǹ�Ŀ¼û�и�Ŀ¼�ļ�, ���Բ���Ҫclose
	while (currfd) {
		my_close(currfd);		// �ر� myfsys �ļ�
	}


	// �����û����ļ����ͷ����ڴ�ռ�
	FILE *fp = fopen(FileName, "w");
	// �ͷ�������̿ռ�
	fwrite(myvhard, SIZE, 1, fp);
	fclose(fp);
}



