/*
 * fieldtools.t
 *
 * Created on: 31 Aug 2012
 * Author: hschmitz
 * Email: holger@notjustphysics.com
 *
 * Copyright 2012 Holger Schmitz
 *
 * This file is part of Schnek.
 *
 * Schnek is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Schnek is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Schnek.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../grid/domain.hpp"
#include <boost/foreach.hpp>

namespace schnek
{

template<
  typename T,
  int rank,
  template<int> class GridCheckingPolicy,
  template<int> class ArrayCheckingPolicy,
  template<typename, int> class StoragePolicy
>
void fill_field(
    Field<T, rank, GridCheckingPolicy, StoragePolicy> &field,
    Array<double, rank, ArrayCheckingPolicy> &coords,
    T &value,
    DependencyUpdater &updater)
{
  RecDomain<rank> domain(field.getLow(), field.getHigh());

  typename RecDomain<rank>::iterator it = domain.begin();
  typename RecDomain<rank>::iterator end = domain.end();
  while (it != end)
  {
    const typename RecDomain<rank>::LimitType &pos=*it;
    for (int i=0; i<rank; ++i) coords[i] = field.indexToPosition(i,pos[i]);
    updater.update();
    field.get(pos) = value;
    ++it;
  }

}


}

