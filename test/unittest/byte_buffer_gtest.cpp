#include <gtest/gtest.h>
#include <data_structures/buffers/byte_buffer.hpp>
#include <data_structures/base/enum.hpp>

using namespace ccl::ds::buffers;
using namespace ccl::ds;

TEST(ByteBufferTest, DefaultConstructor) {
    ByteBuffer buffer;
    EXPECT_EQ(buffer.getBufferCapacity(), 0);
    EXPECT_EQ(buffer.getBufferSize(), 0);
    EXPECT_EQ(buffer.position(), 0);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST(ByteBufferTest, AllocateBuffer) {
    ByteBuffer buffer;
    buffer.allocate(10);
    EXPECT_EQ(buffer.getBufferCapacity(), 10);
    EXPECT_EQ(buffer.getRemainingCapacity(), 10);
}

TEST(ByteBufferTest, PutAndGetByte) {
    ByteBuffer buffer(10);
    buffer.put(0x42);
    buffer.position(0);
    EXPECT_EQ(buffer.get(), 0x42);
}

TEST(ByteBufferTest, PutAndGetUnsignedShort) {
    ByteBuffer buffer(10);
    buffer.putUnsignedShort(0x1234);
    buffer.position(0);
    EXPECT_EQ(buffer.getUnsignedShort(), 0x1234);
}

TEST(ByteBufferTest, PutAndGetUnsignedInt) {
    ByteBuffer buffer(10);
    buffer.putUnsignedInt(0x12345678);
    buffer.position(0);
    EXPECT_EQ(buffer.getUnsignedInt(), 0x12345678);
}

TEST(ByteBufferTest, PutAndGetUnsignedLong) {
    ByteBuffer buffer(20);
    buffer.putUnsignedLong(0x123456789ABCDEF0);
    buffer.position(0);
    EXPECT_EQ(buffer.getUnsignedLong(), 0x123456789ABCDEF0);
}

TEST(ByteBufferTest, PutBufferAndGetBuffer) {
    ByteBuffer buffer(10);
    unsigned char data[] = {0x11, 0x22, 0x33};
    buffer.putBuffer(data, 3);
    buffer.position(0);

    unsigned char output[3] = {0};
    buffer.getBuffer(output, 3);

    EXPECT_EQ(output[0], 0x11);
    EXPECT_EQ(output[1], 0x22);
    EXPECT_EQ(output[2], 0x33);
}

TEST(ByteBufferTest, ClearBuffer) {
    ByteBuffer buffer(10);
    buffer.put(0x11);
    buffer.clear();
    EXPECT_EQ(buffer.getBufferSize(), 0);
    EXPECT_EQ(buffer.position(), 0);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST(ByteBufferTest, ByteOrderSetGet) {
    ByteBuffer buffer;
    buffer.setByteOrder(ByteOrder::BigEndian);
    EXPECT_EQ(buffer.getByteOrder(), ByteOrder::BigEndian);
}

TEST(ByteBufferTest, CopyConstructor) {
    ByteBuffer original(10);
    original.put(0x77);
    ByteBuffer copy(original);
    copy.position(0);
    EXPECT_EQ(copy.get(), 0x77);
}

TEST(ByteBufferTest, MoveConstructor) {
    ByteBuffer original(10);
    original.put(0x99);
    ByteBuffer moved(std::move(original));
    moved.position(0);
    EXPECT_EQ(moved.get(), 0x99);
}

TEST(ByteBufferTest, AssignmentOperatorCopy) {
    ByteBuffer a(10);
    a.put(0xAB);
    ByteBuffer b;
    b = a;
    b.position(0);
    EXPECT_EQ(b.get(), 0xAB);
}

TEST(ByteBufferTest, AssignmentOperatorMove) {
    ByteBuffer a(10);
    a.put(0xCD);
    ByteBuffer b;
    b = std::move(a);
    b.position(0);
    EXPECT_EQ(b.get(), 0xCD);
}
