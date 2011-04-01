USE [earlgrey_test]
GO
ALTER TABLE [dbo].[simple_table_1] DROP CONSTRAINT [DF_simple_table_1_name]
GO
/****** Object:  StoredProcedure [dbo].[SimpleProcedure]    Script Date: 03/31/2011 18:40:16 ******/
DROP PROCEDURE [dbo].[SimpleProcedure]
GO
/****** Object:  Table [dbo].[single_column_table]    Script Date: 03/31/2011 18:40:15 ******/
DROP TABLE [dbo].[single_column_table]
GO
/****** Object:  Table [dbo].[simple_table_1]    Script Date: 03/31/2011 18:40:15 ******/
DROP TABLE [dbo].[simple_table_1]
GO
/****** Object:  FullTextCatalog [simle_table_1_catalogue]    Script Date: 03/31/2011 18:40:14 ******/
EXEC dbo.sp_fulltext_catalog @ftcat=N'simle_table_1_catalogue', @action=N'drop'
GO
/****** Object:  FullTextCatalog [simle_table_1_catalogue]    Script Date: 03/31/2011 18:40:14 ******/
CREATE FULLTEXT CATALOG [simle_table_1_catalogue]WITH ACCENT_SENSITIVITY = ON
AUTHORIZATION [dbo]
GO
/****** Object:  Table [dbo].[simple_table_1]    Script Date: 03/31/2011 18:40:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[simple_table_1](
	[unique_key] [int] NOT NULL,
	[name] [varchar](45) NOT NULL,
	[when] [datetime] NOT NULL,
	[interval] [datetime] NOT NULL,
 CONSTRAINT [PK_simple_table_1] PRIMARY KEY CLUSTERED 
(
	[unique_key] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
CREATE NONCLUSTERED INDEX [IX_simple_table_1] ON [dbo].[simple_table_1] 
(
	[unique_key] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
GO
CREATE FULLTEXT INDEX ON [dbo].[simple_table_1](
[name] LANGUAGE [Korean])
KEY INDEX [PK_simple_table_1]ON ([simle_table_1_catalogue], FILEGROUP [PRIMARY])
WITH (CHANGE_TRACKING = AUTO, STOPLIST = SYSTEM)
GO
INSERT [dbo].[simple_table_1] ([unique_key], [name], [when], [interval]) VALUES (0, N'가', CAST(0x00009D2A0167B974 AS DateTime), CAST(0xFFFFFFFE00000000 AS DateTime))
INSERT [dbo].[simple_table_1] ([unique_key], [name], [when], [interval]) VALUES (1, N'나', CAST(0x00009E970167BAA0 AS DateTime), CAST(0x000000000167B974 AS DateTime))
INSERT [dbo].[simple_table_1] ([unique_key], [name], [when], [interval]) VALUES (2, N'다', CAST(0x0000A0040167BBCC AS DateTime), CAST(0x000000030012F714 AS DateTime))
/****** Object:  Table [dbo].[single_column_table]    Script Date: 03/31/2011 18:40:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[single_column_table](
	[unique_key] [int] NOT NULL,
 CONSTRAINT [PK_single_column_table] PRIMARY KEY CLUSTERED 
(
	[unique_key] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
INSERT [dbo].[single_column_table] ([unique_key]) VALUES (100)
/****** Object:  Trigger [SimpleTrigger]    Script Date: 03/31/2011 18:40:16 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TRIGGER [dbo].[SimpleTrigger] 
   ON  [dbo].[simple_table_1] 
   AFTER INSERT,DELETE,UPDATE
AS 
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for trigger here

END
GO
/****** Object:  StoredProcedure [dbo].[SimpleProcedure]    Script Date: 03/31/2011 18:40:16 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[SimpleProcedure] 
	-- Add the parameters for the stored procedure here
	@Count int = 0
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	-- Insert statements for procedure here
	SET ROWCOUNT @Count
	SELECT * FROM dbo.simple_table_1
	SET ROWCOUNT 0 
END
GO
/****** Object:  Default [DF_simple_table_1_name]    Script Date: 03/31/2011 18:40:15 ******/
ALTER TABLE [dbo].[simple_table_1] ADD  CONSTRAINT [DF_simple_table_1_name]  DEFAULT ('') FOR [name]
GO
