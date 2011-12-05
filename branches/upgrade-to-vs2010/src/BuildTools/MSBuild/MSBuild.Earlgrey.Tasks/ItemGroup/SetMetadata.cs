using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.ItemGroup
{
    /// <summary>
    /// Using an ItemGroup A as new metadata values of the other ItemGroup B, create new ItemGroup C with new metadata.
    /// </summary>
    /// <example>
    /// <code title="Examples" lang="xml" source=".\Samples\msbuild-SetMetadata.xml" />
    /// </example>
    /// <remarks></remarks>
    public class SetMetadata : AbstractTask
    {
        private readonly List<ITaskItem> _outputTasks;

        /// <inheritdoc />
        public SetMetadata()
        {
            _outputTasks = new List<ITaskItem>();
        }

        /// <inheritdoc />
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

        /// <inheritdoc />
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

        /// <summary>
        /// [Required] The source ItemGroup, whose all of internal values will be preserved.
        /// </summary>
        /// <value>Gets or sets the items.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem[] Items { get; set; }

        /// <summary>
        /// [Required] The key name of the new metadata.
        /// </summary>
        /// <value>Gets or sets the name of the key.</value>
        /// <remarks></remarks>
        [Required]
        public string KeyName { get; set; }

        /// <summary>
        /// [Required] The ItemGroup to be used as metadata values of newly-created ItemGroup.
        /// </summary>
        /// <value>Gets or sets the new metadata values.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem[] Values { get; set; }

        /// <summary>
        /// [Output] Newly-created ItemGroup with new metadata.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] OutputTasks
        {
            get { return _outputTasks.ToArray(); }
        }

    }
}
