# memlab: A memory latency and bandwidth benchmark

```
	memlab - MEMory Latency And Benchmark
	usage: ./memlab  [options] [test] [test] [...]
	options:
	     -h     help
	     -p     plot (format output for gnuplot)
	     -q     sqlplot (format output for sqlplot)
	     -v     verbose (include system infos)
	     -rN    range N Bytes (eg. 8192, 8K, 2M, 1G)
	     -dN    detail for iteration: 2^N steps between Powers-of-two
	     -tN    create N threads
	tests:
	     lat : latency range/stride
	     bw  : bandwidth test
	     bwt : threaded bandwidth test
```