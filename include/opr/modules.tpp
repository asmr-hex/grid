#ifndef OPR_MODULES_TPP
#define OPR_MODULES_TPP


template <typename T>
Sequence<T>::Sequence(sequence_t sequence) : sequence(sequence) {};

template <typename T>
rx::observable<T> Sequence<T>::events() {
  return events_subj.get_observable();
}

template <typename T>
rx::observable<T> Sequence<T>::added() {
  return added_subj.get_observable();
}

template <typename T>
rx::observable<T> Sequence<T>::removed() {
  return removed_subj.get_observable();
}

template <typename T>
void Sequence<T>::sync(rx::observable<step_idx_t> observable) {
  // TODO unsync if a subscription already exists

  observable
    .subscribe([this] (step_idx_t index) {
      events_subj.get_subscriber().on_next(at(index));
    });
}

template <typename T>
void Sequence<T>::unsync() {
  subscription.unsubscribe();
}

template <typename T>
int Sequence<T>::size() {
  return sequence.size();
}

template <typename T>
typename Sequence<T>::data_t Sequence<T>::at(step_idx_t step) noexcept {
  data_t results;

  try {
    auto data_at_step = sequence.at(step);
    for (auto itr : data_at_step) {
      results.insert(results.end(), itr.second.begin(), itr.second.end());
    }
  } catch (std::out_of_range &error) {
    // no events at this step...carry on.
  }

  return results;
};

template <typename T>
typename Sequence<T>::data_t Sequence<T>::at(step_idx_t step, step_data_id_t id) noexcept {
  data_t results;

  try {
    auto data_at_step = sequence.at(step);
    try {
      results = data_at_step.at(id);
    } catch (std::out_of_range &error) { /* no data with  this data id...carry on. */ }
  } catch (std::out_of_range &error) { /* no data at this step...carry on. */ }
  
  return results;
};

template <typename T>
typename Sequence<T>::data_t Sequence<T>::at(step_idx_t step, const std::vector<step_data_id_t>& ids) noexcept {
  data_t results;

  try {
    auto data_at_step = sequence.at(step);
    for (auto id : ids) {
      try {
        results.insert(results.end(), data_at_step.at(id).begin(), data_at_step.at(id).end());
      } catch (std::out_of_range &error) { /* no data with  this data id...carry on. */ } 
    }
  } catch (std::out_of_range &error) { /* no data at this step...carry on. */ }
  
  return results;
};

template <typename T>
typename Sequence<T>::step_t Sequence<T>::operator[](step_idx_t step) {
  return sequence[step];
}

template <typename T>
void Sequence<T>::insert(step_idx_t idx, step_t step) {
  sequence[idx] = step;
}

template <typename T>
void Sequence<T>::merge(step_idx_t idx, step_t step) {
  for (auto itr : step) {
    sequence[idx][itr.first] = itr.second;
  }
}

template <typename T>
void Sequence<T>::clear() {
  sequence.clear();
}

template <typename T>
void Sequence<T>::clear(step_idx_t idx) {
  sequence.erase(idx);
}

template <typename T>
void Sequence<T>::clear(step_idx_t idx, step_data_id_t id) {
  sequence[idx].erase(id);
}


#endif
