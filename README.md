# project06-achandrashekhar
project06-achandrashekhar created by GitHub Classroom
I am using 3 pipes, there is a pipe between the parent and the middle process too, (fds2) this pipe writes the analysis data (metrics) to the parent and the parent just reads from this pipe.

I support two pipes for this project.


Running the project :
please add a \ before the pipes like \\|

`./new2 seq 10000 \\| wc -l`

For example : 


Aishwaryas-Air:~ ashi$ gcc -o new2 new2.c


Aishwaryas-Air:~ ashi$ ./new2 seq 10000 \\| wc -l

This will generate pa.log file
**PLEASE NOTE THAT LARGE NUMBERS LIKE 100000 WILL TAKE A FEW SECONDS BECAUSE THERE IS SO MUCH DATA BEING TRANSFERRED***** 
