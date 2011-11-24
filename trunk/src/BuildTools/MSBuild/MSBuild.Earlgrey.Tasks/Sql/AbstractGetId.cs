using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Text;
using System.Linq;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Sql
{
    /// <summary>
    /// Defines the interface for the classes to retrieve some identity numbers from SQL Server.
    /// </summary>
    /// <remarks></remarks>
    public abstract class AbstractGetId : AbstractTask
    {
        private readonly List<ITaskItem> _identities;


        /// <summary>
        /// [Required] Gets or sets the connection string.
        /// </summary>
        /// <value>The connection string.</value>
        /// <remarks></remarks>
        [Required]
        public string ConnectionString { get; set; }

        /// <summary>
        /// [Required] The names of the target objects or users.
        /// </summary>
        /// <value>Gets or sets the names.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem[] Names { get; set; }

        /// <summary>
        /// [Output] Gets the identity values of the <see cref="Names"/>.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public ITaskItem[] Ids
        {
            get { return _identities.ToArray(); }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <remarks></remarks>
        protected AbstractGetId()
        {
            _identities = new List<ITaskItem>();
        }

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            if (Names.Any(item => string.IsNullOrEmpty(item.ItemSpec) == true))
            {
                Log.LogError("Every object name should have a name!");
                return false;
            }

            return true;
        }


        /// <summary>
        /// Gets the command string.
        /// </summary>
        /// <param name="item">The item.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        protected abstract string GetCommandString(ITaskItem item);


        /// <inheritdoc />
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

                        // ITaskItem newItem = new TaskItem(name);
                        ITaskItem newItem = new TaskItem(name.ItemSpec);                        
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
