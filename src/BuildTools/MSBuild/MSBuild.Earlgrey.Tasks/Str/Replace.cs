using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Str
{
    public class Replace : AbstractTask
    {
        protected override bool ValidateParameters()
        {
            return true;
        }

        protected override bool ExecuteCommand()
        {
            return true;
        }


        [Required]
        public ITaskItem[] Items { get; set; }

        [Output]
        public ITaskItem[] ParentFolder
        {
            get
            {
                var result = from item in Items
                             select new TaskItem(System.IO.Directory.GetParent(item.ItemSpec).FullName);
                ;
                return result.ToArray();
            }
        }
    }
}
