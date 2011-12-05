using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO
{
    /// <summary>
    /// Provides some useful functionalities needed to get additional information related to paths.
    /// </summary>
     /// <example>
    /// <code title="How to get parent folders" lang="xml" source=".\Samples\msbuild-pathhelper.xml" />
    /// </example>
    /// <remarks></remarks>
    public class PathHelper : AbstractTask
    {
        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            return true;
        }

        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            return true;
        }


        /// <summary>
        /// [Required] Gets or sets the source file/folder paths.
        /// </summary>
        /// <value>The items.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem[] Items { get; set; }

        /// <summary>
        /// [Output] Gets the parent folder of the source <see cref="Items"/>.
        /// </summary>
        /// <remarks></remarks>
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
