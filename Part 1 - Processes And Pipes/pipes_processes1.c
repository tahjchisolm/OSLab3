// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu";  
    char input_str[100]; 
    pid_t p;
  
    // creating variables for second pipe
    char fixed_str2[] = "gobison.org";
    char input_str2[100];
    //pid_t q;
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate: ");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
        close(fd2[1]);
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child to send a string 
        // printf("wait\n");
        wait(NULL); 
        //printf("wait done\n");
      
        close(fd1[1]); // goal: close writing end of pipes to read
        //close(fd2[1]);
      
        // add string?
        // goal: read string using second pipe 
        char concat_str2[100];
        read(fd2[0], concat_str2, 100);
        //printf("%s\n", concat_str2);
      
        // goal: concatenate fixed string "gobison.org"
        int l = strlen(concat_str2);
        int i;
        for (i=0; i<strlen(fixed_str2); i++)
          concat_str2[l++] = fixed_str2[i];
      
        concat_str2[l] = '\0';
        printf("Second Concatenated string %s\n", concat_str2);
        // printf("%s\n", concat_str); //  concat_str undeclared
  
        close(fd2[1]); // Close writing end of pipes 
        close(fd1[1]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipe
        close(fd2[0]); 
      
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
        // printf("%s\n", concat_str); // concat_str = input_str
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Concatenated string %s\n", concat_str);
        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[0]); 
      
        printf("Enter another string to concatenate: ");
        scanf("%s", input_str2);
      
        // goal: Write concat_str and close writing end of second pipe 
        write(fd2[1], input_str2, strlen(input_str2)+1);
        // concat_str = input_str + fixed_str[i]
        // close(fd2[1]);

        // printf("exit\n");
        exit(0); 
    } 
  
       
      
}