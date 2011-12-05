using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.ItemGroup
{
    /// <summary>
    /// <see cref="ItemsHelper"/> consists of basic grouping functionalities for an ItemGroup. For instance, it can get the first item of the ItemGroup list.
    /// </summary>
    /// <example>
    /// <code title="Examples" lang="xml" source=".\Samples\msbuild-itemshelper.xml" />
    /// </example>
    /// <remarks></remarks>
    public class ItemsHelper : AbstractTask
    {
        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            return true;
        }

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            return true;
        }

        /// <summary>
        /// [Required] Gets or sets the items.
        /// </summary>
        /// <value>The items.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem[] Items { get; set; }

        /// <summary>
        /// [Output] Gets the first item of <see cref="Items"/>.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem First
        {
            get { return Items.First(); }
        }

        /// <summary>
        /// [Output] Gets the last item of <see cref="Items"/>.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem Last
        {
            get { return Items.Last(); }
        }

        /// <summary>
        /// [Output] Gets the number of <see cref="Items"/>.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public int Count
        {
            get { return Items.Count(); }
        }

        /// <summary>
        /// [Output] Gets the item with maximum value in <see cref="Items"/>.
        /// </summary>
        /// <remarks></remarks>
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

        /// <summary>
        /// [Output] Gets the item with minimum value in <see cref="Items"/>.
        /// </summary>
        /// <remarks></remarks>
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

        /// <summary>
        /// [Output] Gets the distinct items among <see cref="Items"/>.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] Distinct
        {
            get { return Items.Distinct().ToArray(); }
        }
    }
}
