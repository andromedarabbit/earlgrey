using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Sql
{
    /// <summary>
    /// Getting the user ids from SQL Server. 
    /// </summary>
    /// <remarks></remarks>
    public class GetUserId : AbstractGetId
    {
        /// <inheritdoc />
        protected override string GetCommandString(ITaskItem item)
        {
            return string.Format("SELECT USER_ID(N'{0}')", item.ItemSpec);
        }

    }
}
