#include "lps22.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lps22 {

//Constans
static const char *const TAG = "lps22.sensor";


void LPS22Component::setup() {

}

void LPS22Component::dump_config() {

}

float LPS22Component::get_setup_priority() const { return setup_priority::DATA; }

void LPS22Component::update() {

}


}  // namespace lps22
}  // namespace esphome