#!/bin/bash

mkdir protobuf-generated/

DIRS=""
FILES=""

for i in `find tensorflow | grep .proto$`
 do
  FILES+=" ${i}"
 done

#echo $DIR
echo $FILES
./bazel-out/host/bin/google/protobuf/protoc --proto_path=./google/protobuf/src  --proto_path=. --cpp_out=protobuf-generated/ $FILES
