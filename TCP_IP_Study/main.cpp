#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <cstdlib>
#include <sys/stat.h>
#include <iostream>

int GetLen(const char* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		++i;
	}
	return i;
}

void CopyLow(const char* src, const char* dst)
{
	int fd, fd2;
	char buf[100];

	fd = _open(src, O_RDONLY | O_TEXT);
	fd2 = _open(dst, O_WRONLY | O_CREAT | O_TEXT, S_IWRITE);

	if (fd == -1 || fd2 == -1)
	{
		printf("Invalid File");
		exit(1);
	}

	int err = 0;
	while (!_eof(fd))
	{
		err = _read(fd, buf, sizeof(buf));
		if (err == -1)
		{
			printf("_read error");
			exit(1);
		}

		int len = GetLen(buf);
		_write(fd2, buf, len + 1);
	}

	_close(fd);
	_close(fd2);
}

void CopyANSI(const char* src, const char* dst)
{
	FILE* rfp = nullptr;
	FILE* wfp = nullptr;

	fopen_s(&rfp, src, "r");
	fopen_s(&wfp, dst, "w");

	if (rfp == nullptr || wfp == nullptr)
	{
		printf("fopen err");
		exit(1);
	}

	char ch;
	while (!feof(rfp))
	{
		ch = fgetc(rfp);
		fputc(ch, wfp);
	}

	fclose(rfp);
	fclose(wfp);

	return;
}

void WriteLow()
{
	int fd;
	char buf[] = "Let's go!\n";

	fd = _open("data.txt", O_WRONLY | _O_CREAT | O_TRUNC, S_IWRITE);

	if (fd == -1)
	{
		printf("open() error");
		exit(1);
	}

	if (!_write(fd, buf, sizeof(buf)))
	{
		printf("write() error");
		exit(1);
	}

	_close(fd);
	return;
}

void ReadLow()
{
	int fd;
	char buf[100];

	fd = _open("data.txt", O_RDONLY);

	if (fd == -1)
	{
		printf("open() error");
		exit(1);
	}

	if (!_read(fd, buf, sizeof(buf)))
	{
		printf("read() error");
		exit(1);
	}

	printf("file data : %s", buf);
	_close(fd);
}

int main()
{
	CopyANSI("TestData.txt", "TestData2.txt");
	return 0;
}