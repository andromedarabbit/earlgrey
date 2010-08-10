using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.ItemGroup
{
    public class ItemsHelper : AbstractTask
    {
        protected override bool ExecuteCommand()
        {
            return true;
        }

        protected override bool ValidateParameters()
        {
            return true;
        }

        [Required]
        public ITaskItem[] Items { get; set; }

        [Output]
        public ITaskItem First
        {
            get { return Items.First(); }
        }

        [Output]
        public ITaskItem Last
        {
            get { return Items.Last(); }
        }

        [Output]
        public int Count
        {
            get { return Items.Count(); }
        }

        [Output]
        public ITaskItem[] Max
        {
            get
            {
                var result = from item in Items
                             let max = Items.Max(i => i.ItemSpec)
                             where item.ItemSpec == max
                             select item
                             ;
                return result.ToArray();
            }
        }

        [Output]
        public ITaskItem[] Min
        {
            get
            {
                var result = from item in Items
                             let min = Items.Min(i => i.ItemSpec)
                             where item.ItemSpec == min
                             select item
                             ;
                return result.ToArray();
            }
        }

    }
}
