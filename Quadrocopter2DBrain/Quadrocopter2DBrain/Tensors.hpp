//
//  Tensors.hpp
//  QuadrocopterBrain
//
//  Created by anton on 13/02/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef Tensors_hpp
#define Tensors_hpp

#include "tensorflow/core/public/session.h"
//#include "tensorflow/core/public/env.h"

template<typename T>
void printTensor (const tensorflow::Tensor& t);

/**
enum DataType {
  DT_INVALID = 0,
  DT_FLOAT = 1,
  DT_DOUBLE = 2,
  DT_INT32 = 3,
  DT_UINT8 = 4,
  DT_INT16 = 5,
  DT_INT8 = 6,
  DT_STRING = 7,
  DT_COMPLEX64 = 8,
  DT_INT64 = 9,
  DT_BOOL = 10,
  DT_QINT8 = 11,
  DT_QUINT8 = 12,
  DT_QINT32 = 13,
  DT_BFLOAT16 = 14,
  DT_QINT16 = 15,
  DT_QUINT16 = 16,
  DT_UINT16 = 17,
  DT_COMPLEX128 = 18,
  DT_HALF = 19,
  DT_FLOAT_REF = 101,
  DT_DOUBLE_REF = 102,
  DT_INT32_REF = 103,
  DT_UINT8_REF = 104,
  DT_INT16_REF = 105,
  DT_INT8_REF = 106,
  DT_STRING_REF = 107,
  DT_COMPLEX64_REF = 108,
  DT_INT64_REF = 109,
  DT_BOOL_REF = 110,
  DT_QINT8_REF = 111,
  DT_QUINT8_REF = 112,
  DT_QINT32_REF = 113,
  DT_BFLOAT16_REF = 114,
  DT_QINT16_REF = 115,
  DT_QUINT16_REF = 116,
  DT_UINT16_REF = 117,
  DT_COMPLEX128_REF = 118,
  DT_HALF_REF = 119,
  DataType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  DataType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
*/
template<typename T>
void printTensorVector (const tensorflow::Tensor& t);


template<typename T>
void printTensor (const tensorflow::Tensor& t) {
using namespace tensorflow;
	int64 sizei = t.shape().dim_size(0);
	int64 sizej = t.shape().dim_size(1);
	auto m = t.matrix<T>();
	for (int i=0; i<sizei; i++) {
		for (int j=0; j<sizej; j++) {
			std::cerr << m (i, j) << " ";
		}
		std::cerr << std::endl;
	}
}

template<typename T>
void getTensorValues (const tensorflow::Tensor& t, std::vector<T>& values) {
using namespace tensorflow;
	int64 sizei = t.shape().dim_size(0);
	int64 sizej = t.shape().dim_size(1);
	auto m = t.matrix<T>();
	for (int i=0; i<sizei; i++) {
		for (int j=0; j<sizej; j++) {
			values [i * sizei + j] = m (i, j);
		}
	}
}

template<typename T>
void getTensorValues (const tensorflow::Tensor& t, std::vector<T>& values, int tensorIndex) {
using namespace tensorflow;
	int64 sizej = t.shape().dim_size(1);
	auto m = t.matrix<T>();
	for (int j=0; j<sizej; j++) {
		values [j] = m (tensorIndex, j);
	}
}

template<typename T>
void printTensorVector (const tensorflow::Tensor& t) {
using namespace tensorflow;
	int64 sizei = t.shape().dim_size(0);
	auto m = t.flat<T>();
	for (int i=0; i<sizei; i++) {
		std::cerr << m (i) << " ";
	}
	std::cerr << std::endl;
}

template<typename T>
void getTensorVectorValues (const tensorflow::Tensor& t, std::vector<T>& values) {
using namespace tensorflow;
	values.clear ();
	int64 sizei = t.shape().dim_size(0);
	auto m = t.flat<T>();
	for (int i=0; i<sizei; i++) {
		values.push_back (m (i));
	}
}

template<typename T>
void fillTensor (tensorflow::Tensor& t, const std::vector<T>& values, int tensorIndex) {
	int s = values.size ();
	for (int i=0; i<s; i++) {
		t.matrix<T>()(tensorIndex, i) = values [i];
	}
}


#endif /* Tensors_hpp */
