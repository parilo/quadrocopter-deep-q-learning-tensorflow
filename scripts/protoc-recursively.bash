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
./bazel-out/host/bin/external/protobuf/protoc --proto_path=./bazel-Tensorflow/external/protobuf/src  --proto_path=. --cpp_out=protobuf-generated/ $FILES
