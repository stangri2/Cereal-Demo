#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <sstream>

struct TradeMessage {
  uint64_t timestamp;
  uint32_t price;
  uint32_t volume;
  char buyer_mpid[10];
  char buyer_order_id[10];
  char seller_mpid[10];
  char seller_order_id[10];
  char trade_id[10];

  template<class Archive>
  void serialize(Archive& ar) {
    ar(timestamp, price, volume);
    ar(cereal::binary_data(buyer_mpid, sizeof(buyer_mpid)));
    ar(cereal::binary_data(buyer_order_id, sizeof(buyer_order_id)));
    ar(cereal::binary_data(seller_mpid, sizeof(seller_mpid)));
    ar(cereal::binary_data(seller_order_id, sizeof(seller_order_id)));
    ar(cereal::binary_data(trade_id, sizeof(trade_id)));
  }
};

int main() {
    // Create a TradeMessage object with test data
    TradeMessage outMsg;
    outMsg.timestamp = 1234567890;
    outMsg.price = 10000;
    outMsg.volume = 200;
    std::memcpy(outMsg.buyer_mpid, "BMPID12345", 10);
    std::memcpy(outMsg.buyer_order_id, "BORDID6789", 10);
    std::memcpy(outMsg.seller_mpid, "SMPID12345", 10);
    std::memcpy(outMsg.seller_order_id, "SORDID6789", 10);
    std::memcpy(outMsg.trade_id, "TRADEID001", 10);

    // Serialize the object to a stringstream
    std::stringstream ss;
    {
        cereal::BinaryOutputArchive oarchive(ss);
        // Serialize using binary_data to control the exact amount of data written
        oarchive(cereal::binary_data(&outMsg, sizeof(TradeMessage)));
    }

    // Deserialize into a new TradeMessage object
    TradeMessage inMsg;
    {
        cereal::BinaryInputArchive iarchive(ss);
        // Deserialize using binary_data to control the exact amount of data read
        iarchive(cereal::binary_data(&inMsg, sizeof(TradeMessage)));
    }

    // Now verify that the inMsg has the same data as outMsg
    assert(inMsg.timestamp == outMsg.timestamp);
    assert(inMsg.price == outMsg.price);
    assert(inMsg.volume == outMsg.volume);
    assert(std::strncmp(inMsg.buyer_mpid, outMsg.buyer_mpid, sizeof(outMsg.buyer_mpid)) == 0);
    assert(std::strncmp(inMsg.buyer_order_id, outMsg.buyer_order_id, sizeof(outMsg.buyer_order_id)) == 0);
    assert(std::strncmp(inMsg.seller_mpid, outMsg.seller_mpid, sizeof(outMsg.seller_mpid)) == 0);
    assert(std::strncmp(inMsg.seller_order_id, outMsg.seller_order_id, sizeof(outMsg.seller_order_id)) == 0);
    assert(std::strncmp(inMsg.trade_id, outMsg.trade_id, sizeof(outMsg.trade_id)) == 0);

    std::cout << "Test passed: All serialized and deserialized values match." << std::endl;
    return 0;
}