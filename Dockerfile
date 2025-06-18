FROM alpine:latest AS build                                                                                           

RUN apk update && \                                                                                                   
    apk add --no-cache \                                                                                              
    cmake \                                                                                                       
    git \                                                                                                         
    build-base                                                                                                    

RUN git clone https://github.com/xirzo/httpparser \                                                                   
    /tmp/httpparser && \                                                                                          
    mkdir -p /tmp/httpparser/build && \                                                                           
    cd /tmp/httpparser/build && \                                                                                 
    cmake -DHTTP_PARSER_BUILD_EXAMPLES=OFF -DHTTP_PARSER_BUILD_TESTS=OFF ..  && \                                 
    cmake --build . && \                                                                                          
    make install  

RUN git clone https://github.com/xirzo/httpserver \                                                                   
    /tmp/httpserver && \                                                                                          
    mkdir -p /tmp/httpserver/build && \                                                                           
    cd /tmp/httpserver/build && \                                                                                 
    cmake ..  && \                                 
    cmake --build . && \                                                                                          
    cmake --install . --prefix /usr/local

WORKDIR /website-in-c
COPY assets/ ./assets/
COPY bin/ ./bin/
COPY lib/ ./lib/
COPY CMakeLists.txt .

WORKDIR /website-in-c/build
RUN cmake .. && \
    cmake --build . 

FROM alpine:latest

RUN addgroup -S website-in-c && \
    adduser -S website-in-c -G website-in-c

WORKDIR /app

USER website-in-c

COPY --chown=website-in-c:website-in-c --from=build \
    ./website-in-c/build/bin/c-website \ 
    ./

COPY --chown=website-in-c:website-in-c --from=build \
    ./website-in-c/assets \ 
    ./assets/

ENTRYPOINT ["./c-website"]
