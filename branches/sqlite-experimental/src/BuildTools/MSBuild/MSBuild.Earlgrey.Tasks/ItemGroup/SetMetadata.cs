using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.ItemGroup
{
    public class SetMetadata : AbstractTask
    {
        private readonly List<ITaskItem> _outputTasks;

        public SetMetadata()
        {
            _outputTasks = new List<ITaskItem>();
        }

        protected override bool ValidateParameters()
        {
            if(Items.Length != Values.Length)
            {
                Log.LogError(
                    string.Format("The number of Items {0} should be equal to the one of Values {1}!", Items.Length, Values.Length)
                    );
                return false;
            }

            if(Items.Length > 0)
            {
                if (Items[0].MetadataNames.Cast<string>().All(itemName => itemName != KeyName) == false)
                {
                    Log.LogError(
                        string.Format("Items already has the key named '{0}'.", KeyName)
                        );
                    return false;
                }
            }

            return true;
        }

        protected override bool ExecuteCommand()
        {
            int count = Items.Length;
            if(count == 0)
                return true;

            _outputTasks.AddRange(Items);

            for (int i = 0; i < count; i++)
            {
                _outputTasks[i].SetMetadata(KeyName, Values[i].ItemSpec);
            }

            return true;
        }

        [Required]
        public ITaskItem[] Items { get; set; }

        [Required]
        public string KeyName { get; set; }

        [Required]
        public ITaskItem[] Values { get; set; }

        [Output]
        public ITaskItem[] OutputTasks
        {
            get { return _outputTasks.ToArray(); }
        }

    }
}
