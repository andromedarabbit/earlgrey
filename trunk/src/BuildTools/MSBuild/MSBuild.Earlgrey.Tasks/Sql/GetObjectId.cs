using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Text;
using System.Linq;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Sql
{
    /// <summary>
    /// Getting the object ids from SQL Server. 
    /// </summary>
    /// <example>
    /// <code title="Getting object identiy numbers from 'master' database." lang="xml" source=".\Samples\msbuild-GetObjectId.xml" />
    /// </example>
    /// <inheritdoc />
    public class GetObjectId : AbstractGetId
    {
        /// <inheritdoc />
        protected override string GetCommandString(ITaskItem item)
        {
            return string.Format("SELECT OBJECT_ID(N'{0}')", item.ItemSpec);
        }

    }
}
