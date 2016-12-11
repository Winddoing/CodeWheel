#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
/* 用于为每个线程保存文件指针的 TSD 键值。 */
static pthread_key_t thread_log_key;
/* 将 MESSAGE 写入当前线程的日志中。 */
void write_to_thread_log (const char* message)
{
	FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);
	fprintf (thread_log, "%s\n" , message);
}
/* 将日志文件指针 THREAD_LOG 关闭。 */
void close_thread_log (void* thread_log)
{
	fclose ((FILE*) thread_log);
}
void* thread_function (void* args)
{
	char thread_log_filename[20];
	FILE* thread_log;
	/* 生成当前线程使用的日志文件名。 */
	sprintf (thread_log_filename, "thread%d.log" , (int) pthread_self ());
	/* 打开日志文件。 */
	thread_log = fopen (thread_log_filename, "w" );
	/* 将文件指针保存在 thread_log_key 标识的 TSD 中。 */
	pthread_setspecific (thread_log_key, thread_log);
	write_to_thread_log ("Thread starting." );
	/* 在这里完成线程任务……*/
	return NULL;
}
int main ()
{
	int i;
	pthread_t threads[5];
	/* 创建一个键值，用于将线程日志文件指针保存在 TSD 中。
	 * 调用 close_thread_log 以关闭这些文件指针。 */
	pthread_key_create (&thread_log_key, close_thread_log);
	/* 创建线程以完成任务。 */
	for (i = 0; i < 5; ++i)
		pthread_create (&(threads[i]), NULL, thread_function, NULL);
	/* 等待所有线程结束。 */
	for (i = 0; i < 5; ++i)
		pthread_join (threads[i], NULL);
	return 0;
}
