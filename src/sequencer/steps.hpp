#ifndef SEQUENCER_STEPS_H
#define SEQUENCER_STEPS_H

#include <map>
#include <set>

#include "../io/io.hpp"
#include "../config/config.hpp"
#include "../animation/types.hpp"
#include "../animation/animator.hpp"


// page_relative_step_t defines a step location relative to the page it is on.
// This is in contrast to a sequence relative step which describes the step
// location with respect to the entire sequence.
struct page_relative_step_t {
  int page;
  int step;
};

class Steps {
public:
  int cursor = 0;
  int last   = 64;
  
  struct {
    bool last_step_and_cursor = false;
    bool last_step_and_step   = false;
    bool cursor_and_step      = false;
  } collision;
  
  Steps(IO *io, Config *config, Animator *animation)
    : io(io), config(config), animation(animation) {};

  void render(int page) {
    // remove all animations on page
    animation->remove()
    
      // render activated steps

      // render cursor

      // render last step if necessary    
  };

  // add a rendered step
  void add(int sequence_relative_step) {
    page_relative_step_t page_relative = get_page_relative_step(sequence_relative_step);

    // store in rendered_steps
    rendered_steps[page_relative.page].insert(page_relative.step);
  };

  // bulk remove multiple rendered steps
  void remove(std::vector<int> sequence_relative_steps) {
    for (int step : sequence_relative_steps) {
      remove(step);
    }
  }

  // remove a rendered step
  void remove(int sequence_relative_step) {
    page_relative_step_t page_relative = get_page_relative_step(sequence_relative_step);
    
    // remove from rendered_steps
    rendered_steps[page_relative.page].erase(page_relative.step);
  }

  // check whether a provided sequence relative step is rendered.
  bool is_rendered(int sequence_relative_step) {
    page_relative_step_t page_relative = get_page_relative_step(sequence_relative_step);
    return is_rendered(page_relative);
  };

  bool is_rendered(page_relative_step_t page_relative) {
    try {
      std::set<int> rendered_steps_on_page = rendered_steps.at(page_relative.page);
      return rendered_steps_on_page.find(page_relative.step) != rendered_steps_on_page.end();
    } catch (std::out_of_range &error) {
      // there was nothing on that page even apparently
      return false;
    }
  };
  
private:
  // steps rendered on each page
  std::map<int, std::set<int> > rendered_steps;

    // led brightness and animation configuration for pages panel
  struct {
    int on        = 10;
    int off       = 0;
    int cursor    = 15;
    waveform last = {.amplitude.max = 15,
                     .amplitude.min = 4,
                     .modulator = { .type = Unit },
                     .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
    };
    struct {
      waveform last_step_and_cursor          = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
      waveform last_step_and_step            = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
      waveform cursor_and_step               = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
      waveform cursor_and_step_and_last_step = {.amplitude.max = 15,
                                                .amplitude.min = 4,
                                                .modulator = { .type = Unit },
                                                .pwm = { .duty_cycle = 0.8, .period = 300, .phase = 0 }
      };
    } collision;
  } led;

  // given a sequence relative step, this converts it to a page relative step.
  page_relative_step_t get_page_relative_step(int sequence_relative_step) {
    return { .page = sequence_relative_step / config->mappings.steps.get_area(),
             .step = sequence_relative_step % config->mappings.steps.get_area(),
    };
  };
  
};

#endif
