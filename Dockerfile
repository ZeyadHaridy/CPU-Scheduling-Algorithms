FROM ubuntu:22.04

RUN apt-get update \
    -o Acquire::Check-Valid-Until=false \
    -o Acquire::Check-Date=false && \
    apt-get install -y g++ make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make

CMD ["./main"]
