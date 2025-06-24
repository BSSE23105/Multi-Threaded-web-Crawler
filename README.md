# Multi‑Threaded Web Crawler

## Overview

This C program reads up to 100 URLs from a text file and spawns multiple POSIX threads to perform the following tasks:

- **Download pages:** Fetch each URL over HTTP using **libcurl**.
- **Analyze content:** Count sentences, words, and character frequencies (a–z) in the downloaded content.
- **Aggregate statistics:** Report both per‑URL and global statistics.
- **Measure performance:** Compute the total execution time.

A supplied Python script generates performance graphs.


## Statistics Reported

By default, the crawler computes and prints the following three per‑URL statistics:

- **Sentence count**
- **Word count**
- **Character frequency (a–z)**

After processing all URLs, it also prints the **total execution time**.

## Dependencies

### On WSL (Ubuntu or Debian)

Update the package index and install the required software:
sudo apt update
sudo apt install -y build-essential libcurl4-openssl-dev valgrind gprof python3 python3-pip
Installing Matplotlib for Python
Install Matplotlib using pip:
python3 -m pip install --user matplotlib


`Build Instructions`
There are two methods to build the crawler:

`With Profiling Enabled (gprof)`
Compile the program with profiling enabled:

gcc -O2 -pg -o crawler crawler.c -lcurl -lpthread
-O2 optimizes the code for speed.
-pg generates profiling data for gprof.
-lcurl links against the libcurl library.
-lpthread links against the POSIX threads library.

`Without Profiling Flags`
Compile the program without profiling flags:

gcc -O2 -o crawler crawler.c -lcurl -lpthread

Running the Crawler
To execute the crawler, use the following command:


./crawler urls.txt <num_threads> <chunk_size>
Where:

urls.txt: Input file containing newline‑separated URLs.

<num_threads>: The number of threads (must be ≥ 1).

<chunk_size>: The number of URLs each thread processes per lock acquisition.

Example:
./crawler urls.txt 8 4