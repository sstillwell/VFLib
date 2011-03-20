// Copyright (C) 2008-2011 by Vincent Falco, All rights reserved worldwide.
// This file is released under the MIT License:
// http://www.opensource.org/licenses/mit-license.php

#ifndef __VF_INTERVAL_VFHEADER__
#define __VF_INTERVAL_VFHEADER__

// Represents the half-open interval [m_begin, m_end)
template <class Ty>
class Interval
{
public:
  typedef Ty value_type;

  static const Interval none;

  Interval ()
  {
  }

  Interval (const Ty& begin, const Ty& end)
    : m_begin (begin)
    , m_end (end)
  {
  }

  Interval (const Interval& other)
    : m_begin (other.m_begin)
    , m_end (other.m_end)
  {
  }

  Interval& operator= (const Interval& other)
  {
    m_begin = other.m_begin;
    m_end = other.m_end;
    return *this;
  }

  bool operator== (Interval const& rhs) const
  {
    return (empty() && rhs.empty()) ||
           (m_begin == rhs.m_begin && m_end == rhs.m_end);
  }

  bool operator!= (Interval const& rhs ) const
  {
    return empty() ? (!rhs.empty()) :
                     (rhs.empty() || (m_begin != rhs.m_begin ||
                                      m_end == rhs.m_end));
  }

  Ty const& begin () const { return m_begin; }
  Ty const& end () const { return m_end; }
  Ty count () const { return empty () ? 0 : (m_end-m_begin); } // Lebesque measure
  Ty length () const { return count (); }  // sugar
  Ty distance () const { return count (); } // sugar
  bool empty () const { return m_begin >= m_end; }
  bool notEmpty () const { return m_begin < m_end; }
  void setBegin (Ty const& v) { m_begin = v; }
  void setEnd (Ty const& v) { m_end = v; }

  bool contains (Ty const& v ) const
  {
    return notEmpty () && v >= m_begin && v < m_end;
  }

  bool intersects (Interval const& interval) const
  {
    return notEmpty() && interval.notEmpty() &&
           m_end > interval.m_begin && m_begin < interval.m_end;
  }

  // true iff the union is a single non empty half-open interval
  bool adjoins (Interval const& interval) const
  {
    return (empty() != interval.empty()) ||
           (notEmpty() && m_end >= interval.m_begin
                       && m_begin <= interval.m_end);
  }

  bool disjoint (Interval const& interval) const
  {
    return !intersects (interval);
  }

  // A is a superset of B if B is empty or if A fully contains B
  bool superset_of (Interval const& interval) const
  {
    return interval.empty() ||
           (notEmpty() && m_begin <= interval.m_begin
                       && m_end >= interval.m_end);
  }

  // A is a proper superset of B iff:
  // - A is not empty,
  // - B is empty OR A contains B and A has more than B
  bool proper_superset_of (Interval const& interval) const
  {
    return notEmpty() &&
           (interval.empty() || (
             (m_begin <= interval.m_begin && m_end >  interval.m_end) ||
             (m_begin <  interval.m_begin && m_end >= interval.m_end) ));
  }

  bool subset_of (Interval const& interval) const
  {
    return interval.superset_of (*this);
  }

  bool proper_subset_of (Interval const& interval) const
  {
    return interval.proper_superset_of (*this);
  }

  Interval intersection (const Interval& interval) const
  {
    return Interval (std::max (m_begin, interval.m_begin),
                     std::min (m_end, interval.m_end));
  }

  // Returns the smallest interval that contains both intervals
  Interval simple_union (Interval const& interval ) const
  {
    return Interval(
             std::min (interval.normalized().m_begin, normalized().m_begin),
             std::max (interval.normalized().m_end, normalized().m_end));
  }

  // empty if the union cannot be represented as a single half-open interval
  Interval single_union (Interval const& interval) const
  {
    if (empty())
      return interval;

    else if (interval.empty())
      return *this;

    else if (m_end < interval.m_begin || m_begin > interval.m_end)
      return none;

    else
      return Interval (std::min (m_begin, interval.m_begin),
                       std::max (m_end, interval.m_end));
  }

  // Returns true if the Interval is correctly ordererd
  bool normal () const
  {
    return m_end >= m_begin;
  }

  // Returns a correctly ordered interval
  Interval normalized () const
  {
    if (normal ())
      return *this;
    else
      return Interval (m_end, m_begin);
  }

  // Clamp the value to lie within the interval
  template <typename Tv>
  const Ty clamp (Tv v) const
  {
    // These conditionals are carefully ordered so
    // that if m_begin == m_end, value is assigned m_begin.
    if (v > m_end)
      v = m_end - (std::numeric_limits <Tv>::is_integer ? 1 :
                   std::numeric_limits <Tv>::epsilon());

    if (value < m_begin)
      value = m_begin;

    return value;
  }

private:
  Ty m_begin;
  Ty m_end;
};

template <typename Ty>
const Interval<Ty> Interval<Ty>::none = Interval<Ty>(Ty(), Ty());

#endif