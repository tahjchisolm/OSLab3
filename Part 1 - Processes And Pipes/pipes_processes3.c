#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include<sys/wait.h> 

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2], pipefd2[2];
  pid_t pid, pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};
	
	// make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);
  pipe(pipefd2);

  pid = fork();
	
	if (pid == 0)
    {
      pid2 = fork();
			if (pid2 == 0) {
          // child that handles sort

          // replace standard input with input part of pipe
          dup2(pipefd2[0], 0);

          // close pipes
          close(pipefd[0]);
          close(pipefd[1]);
          close(pipefd2[0]);
          close(pipefd2[1]);

          // execute sort
          execvp(sort_args[0], sort_args);
        }
      else
        {
          dup2(pipefd[0], 0);
          dup2(pipefd2[1], 1);
				
          // close pipes
          close(pipefd[0]);
          close(pipefd[1]);
          close(pipefd2[0]);
          close(pipefd2[1]);

          // execute grep
          execvp(grep_args[0], grep_args);
        }
  }
    else {
      dup2(pipefd[1], 1);
			close(pipefd[0]);
			close(pipefd[1]);
			close(pipefd2[0]);
			close(pipefd2[1]);
      
      execvp(cat_args[0], cat_args);
    }
   //kill(pid);
   wait(NULL);
	
	 return 0;
	
}