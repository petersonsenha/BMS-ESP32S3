#pragma once

#include <Arduino.h>

#include "bms_controller.h"
#include "bms_event_log.h"

namespace bms {

class BmsRuntimeConfigService {
 public:
  explicit BmsRuntimeConfigService(BmsController& controller,
                                   BmsConfig initial_config = kDefaultBmsConfig,
                                   BmsEventLog* event_log = nullptr);

  void begin(Stream& io);
  void poll(Stream& io);

  [[nodiscard]] const BmsConfig& active_config() const;

 private:
  void handle_line(const String& line, Stream& io);
  bool apply_set_command(const String& path, const String& value, String& message);
  void print_help(Stream& io) const;
  void print_config(Stream& io) const;

  static String normalize(String value);
  static bool parse_bool(const String& value, bool& parsed_value);
  static bool parse_u16(const String& value, uint16_t& parsed_value);
  static bool parse_u32(const String& value, uint32_t& parsed_value);
  static bool parse_i16(const String& value, int16_t& parsed_value);
  static bool parse_float(const String& value, float& parsed_value);

  BmsController& controller_;
  BmsConfig active_config_{};
  BmsEventLog* event_log_{nullptr};
  String input_buffer_{};
};

}  // namespace bms
