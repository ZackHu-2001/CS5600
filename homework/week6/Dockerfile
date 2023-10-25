# Use the official Ubuntu base image
FROM ubuntu:latest

# Update package lists and install the GCC compiler
RUN apt-get update && apt-get install -y gcc
RUN apt install python3 -y
RUN apt-get install libpthread-stubs0-dev

# Create a working directory and copy your C program
WORKDIR /app
COPY tlb.c /app/
COPY tlb_oneCPU.c /app/
COPY helper.py /app/

# Compile the C program
RUN gcc -o0 -o tlb tlb.c
# RUN gcc -o0 -o tlb_oneCPU tlb_oneCPU.c -lpthread

# Set the command to run your program
# CMD ["./hw 2 1000"]
