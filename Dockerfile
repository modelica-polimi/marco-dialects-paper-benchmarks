FROM ghcr.io/marco-compiler/marco-prod-debian-12@sha256:3ee734a05a9c42d513c712ef0eba40a3c7003109f7a1133eaab5292def7374a5

RUN apt update -y && apt install -y time nano screen
WORKDIR "/root"

ENV PATH="/root/install/marco/bin:/root/install/csv_exporter/bin:$PATH"
ENV SRC_DIR=/data/src
ENV BUILD_DIR=/output/build
ENV LOG_DIR=/output/log
ENV RESULTS_DIR=/output/results
ENV COMPILE_TIMEOUT=18000
ENV SIMULATE_TIMEOUT=18000
