FROM ghcr.io/marco-compiler/marco-prod-debian-12:24f25a9

RUN apt update -y && apt install -y time nano screen

ENV BUILD_ROOT=/output/build
ENV LOG_ROOT=/output/log
ENV RESULTS_ROOT=/output/results
ENV STATS_ROOT=/output/stats
ENV COMPILE_TIMEOUT=3600
ENV SIMULATE_TIMEOUT=10800
