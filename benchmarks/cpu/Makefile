CXX = g++
CXXFLAGS = -O3 -march=native -std=c++17 \
	-I$(INCLUDE_DIR) \
	-I$(HLS_INCLUDE)

SRC_DIR = ../../src
INCLUDE_DIR = ../../include
HLS_INCLUDE = /tools/Xilinx/Vitis_HLS/2024.1/include

MOD_OBJS = mod_reduction.o modulo_reduction.o 
NTT_OBJS = ntt_trans.o ntt_transform.o 
POLY_OBJS = poly_multiplication.o polynomial_multiplication.o 
ENC_OBJS = enc_dec.o encryption.o
TARGET = mod_bench ntt_bench poly_bench encrypt_decrypt_bench

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# stub
stub.o: stub.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# modulus
mod_bench: $(MOD_OBJS)
	$(CXX) $(CXXFLAGS) $(MOD_OBJS) -o mod_bench

mod_reduction.o: mod_reduction.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

modulo_reduction.o: $(SRC_DIR)/modulo_reduction.cpp
	$(CXX) $(CXXFLAGS) -c $< -o modulo_reduction.o

# ntt
ntt_bench: $(NTT_OBJS) modulo_reduction.o
	$(CXX) $(CXXFLAGS) $(NTT_OBJS) modulo_reduction.o -o ntt_bench

ntt_trans.o: ntt_trans.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

ntt_transform.o: $(SRC_DIR)/ntt_transform.cpp
	$(CXX) $(CXXFLAGS) -c $< -o ntt_transform.o

# poly
poly_bench: $(POLY_OBJS) modulo_reduction.o ntt_transform.o
	$(CXX) $(CXXFLAGS) $(POLY_OBJS) modulo_reduction.o ntt_transform.o -o poly_bench

poly_multiplication.o: poly_multiplication.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

polynomial_multiplication.o: $(SRC_DIR)/polynomial_multiplication.cpp
	$(CXX) $(CXXFLAGS) -c $< -o polynomial_multiplication.o

# encrypt and decrypt
encrypt_decrypt_bench: $(ENC_OBJS) modulo_reduction.o ntt_transform.o polynomial_multiplication.o stub.o
	$(CXX) $(CXXFLAGS) $(ENC_OBJS) modulo_reduction.o ntt_transform.o polynomial_multiplication.o stub.o -o encrypt_decrypt_bench

enc_dec.o: enc_dec.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

encryption.o: $(SRC_DIR)/encryption.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o encryption.o

clean:
	rm -f $(OBJS) $(TARGET)
	rm *.o


