using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Text;
using System.Linq;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Sql
{
    public abstract class AbstractGetId : AbstractTask
    {
        private readonly List<ITaskItem> _identities;


        [Required]
        public string ConnectionString { get; set; }

        [Required]
        public ITaskItem[] Names { get; set; }

        [Output]
        public ITaskItem[] Ids
        {
            get { return _identities.ToArray(); }
        }

        protected AbstractGetId()
        {
            _identities = new List<ITaskItem>();
        }

        protected override bool ValidateParameters()
        {
            if (Names.Any(item => string.IsNullOrEmpty(item.ItemSpec) == true))
            {
                Log.LogError("Every object name should have a name!");
                return false;
            }

            return true;
        }


        protected abstract string GetCommandString(ITaskItem item);
        
            // return string.Format("SELECT OBJECT_ID(N'{0}')", name.ItemSpec);
        

        protected override bool ExecuteCommand()
        {
            try
            {
                using (SqlConnection connection = new SqlConnection(ConnectionString))
                {
                    connection.Open();

                    foreach (ITaskItem name in Names)
                    {
                        string commandStr = GetCommandString(name);
                        SqlCommand command = new SqlCommand(commandStr, connection);

                        object returnValue = command.ExecuteScalar();
                        string objectId = returnValue == null ? string.Empty : returnValue.ToString();

                        ITaskItem newItem = new TaskItem(name);
                        newItem.SetMetadata("Id", objectId);
                        _identities.Add(newItem);
                    }

                }

                return true;
            }
            catch (Exception ex)
            {
                Log.LogErrorFromException(ex);
                return false;
            }
        }
    }
}
