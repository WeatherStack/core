FROM csantve/alpine-vcpkg AS builder

WORKDIR /app

RUN apk add --no-cache linux-headers perl bash pkgconfig

COPY vcpkg.json .

RUN vcpkg install

COPY . .

RUN cmake -S . -B build -G "Ninja" \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake

RUN cmake --build build --parallel $(nproc)

FROM alpine:latest

WORKDIR /app
COPY --from=builder /app/bin/ws_core .
COPY --from=builder /usr/lib/* /usr/lib/

EXPOSE ${PORT}

CMD ["./ws_core"]