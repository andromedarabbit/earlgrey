using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class FilterMerge
    {
        private readonly FilterType _filter;

        public FilterMerge(FilterType filter)
        {
            Debug.Assert(filter != null);
            _filter = filter;
        }

        private IEnumerable<FilterType> Filters
        {
            get
            {
                IEnumerable<FilterType> filters = from item in _filter.Items
                                                  where item is FilterType
                                                  select (FilterType)item
                                              ;
                return filters;
            }
        }

        private IEnumerable<FileType> Files
        {
            get
            {
                IEnumerable<FileType> files = from item in _filter.Items
                                              where item is FileType
                                              select (FileType)item
                                              ;
                return files;
            }
        }

        public  void Merge()
        {
            foreach (var filter in Filters)
            {
                FilterMerge filterMerge = new FilterMerge(filter);
                filterMerge.Merge();
            }

            FilesMerge filesMerge = new FilesMerge(Files);
            filesMerge.Merge();
        }
    }
}
