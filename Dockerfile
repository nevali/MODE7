FROM debian:stable-slim AS build

ENV LANG=C
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && apt-get install -f -y --no-install-recommends build-essential autoconf automake libtool

RUN mkdir /build
WORKDIR /build
RUN --mount=type=bind,target=/src /src/configure --disable-shared && make clean && make all && make check && make install

FROM debian:stable-slim AS mode7
COPY --from=build /opt/MODE7 /opt/MODE7
CMD [ "/opt/MODE7/bin/MODE7" ]
