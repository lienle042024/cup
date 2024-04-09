/*
 * "lp" command for CUPS.
 *
 * Copyright © 2007-2019 by Apple Inc.
 * Copyright © 1997-2007 by Easy Software Products.
 *
 * Licensed under Apache License v2.0.  See the file "LICENSE" for more
 * information.
 */

/*
 * Include necessary headers...
 */

#include <cups/cups-private.h>

/*
 * Local functions.
 */

static int restart_job(const char *command, int job_id);
static int set_job_attrs(const char *command, int job_id, int num_options, cups_option_t *options);
static void usage(void) _CUPS_NORETURN;

/*
 * 'main()' - Parse options and send files for printing.
 */

int main(int argc,	   /* I - Number of command-line arguments */
		 char *argv[]) /* I - Command-line arguments */
{
	int i, j;				 /* Looping vars */
	int job_id;				 /* Job ID */
	char *printer,			 /* Printer name */
		*instance,			 /* Instance name */
		*opt,				 /* Option pointer */
		*val,				 /* Option value */
		*title;				 /* Job title */
	int priority;			 /* Job priority (1-100) */
	int num_copies;			 /* Number of copies per file */
	int num_files;			 /* Number of files to print */
	const char *files[1000]; /* Files to print */
	cups_dest_t *dest;		 /* Selected destination */
	int num_options;		 /* Number of options */
	cups_option_t *options;	 /* Options */
	int end_options;		 /* No more options? */
	int silent;				 /* Silent or verbose output? */
	char buffer[8192];		 /* Copy buffer */

printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);

#ifdef __sun
	printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
	/*
	 * Solaris does some rather strange things to re-queue remote print
	 * jobs.  On bootup, the "lp" command is run as "printd" to re-spool
	 * any remote jobs in /var/spool/print.  Since CUPS doesn't need this
	 * nonsense, we just need to add the necessary check here to prevent
	 * lp from causing boot problems...
	 */

	if ((val = strrchr(argv[0], '/')) != NULL)
		val++;
	else
		val = argv[0];

	if (!strcmp(val, "printd"))
		return (0);
#endif /* __sun */
	printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
	_cupsSetLocale(argv);
	printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
	silent = 0;
	printer = NULL;
	dest = NULL;
	num_options = 0;
	options = NULL;
	num_files = 0;
	title = NULL;
	job_id = 0;
	end_options = 0;

	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--help"))
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			usage();
		}
		else if (argv[i][0] == '-' && argv[i][1] && !end_options)
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			for (opt = argv[i] + 1; *opt; opt++)
			{
				switch (*opt)
				{
				case 'E': /* Encrypt */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
#ifdef HAVE_SSL
					cupsSetEncryption(HTTP_ENCRYPT_REQUIRED);
#else
					_cupsLangPrintf(stderr, _("%s: Sorry, no encryption support."), argv[0]);
#endif /* HAVE_SSL */
					break;

				case 'U': /* Username */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						cupsSetUser(opt + 1);
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;
						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected username after \"-U\" option."), argv[0]);
							usage();
						}

						cupsSetUser(argv[i]);
					}
					break;

				case 'c': /* Copy to spool dir (always enabled) */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					break;

				case 'd': /* Destination printer or class */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						printer = opt + 1;
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected destination after \"-d\" option."), argv[0]);
							usage();
						}

						printer = argv[i];
					}

					if ((instance = strrchr(printer, '/')) != NULL)
						*instance++ = '\0';

					if ((dest = cupsGetNamedDest(CUPS_HTTP_DEFAULT, printer,
												 instance)) != NULL)
					{
						for (j = 0; j < dest->num_options; j++)
							if (cupsGetOption(dest->options[j].name, num_options,
											  options) == NULL)
								num_options = cupsAddOption(dest->options[j].name,
															dest->options[j].value,
															num_options, &options);
					}
					else if (cupsLastError() == IPP_STATUS_ERROR_BAD_REQUEST ||
							 cupsLastError() == IPP_STATUS_ERROR_VERSION_NOT_SUPPORTED)
					{
						_cupsLangPrintf(stderr,
										_("%s: Error - add '/version=1.1' to server "
										  "name."),
										argv[0]);
						return (1);
					}
					else if (cupsLastError() == IPP_STATUS_ERROR_NOT_FOUND)
					{
						_cupsLangPrintf(stderr,
										_("%s: Error - The printer or class does not exist."), argv[0]);
						return (1);
					}
					break;

				case 'f': /* Form */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);

					if (opt[1] != '\0')
					{
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected form after \"-f\" option."), argv[0]);
							usage();
						}
					}
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					_cupsLangPrintf(stderr, _("%s: Warning - form option ignored."), argv[0]);
					break;

				case 'h': /* Destination host */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						cupsSetServer(opt + 1);
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected hostname after \"-h\" option."), argv[0]);
							usage();
						}

						cupsSetServer(argv[i]);
					}
					break;

				case 'i': /* Change job */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						val = opt + 1;
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Expected job ID after \"-i\" option."), argv[0]);
							usage();
						}

						val = argv[i];
					}

					if (num_files > 0)
					{
						_cupsLangPrintf(stderr, _("%s: Error - cannot print files and alter jobs simultaneously."), argv[0]);
						return (1);
					}

					if (strrchr(val, '-') != NULL)
						job_id = atoi(strrchr(val, '-') + 1);
					else
						job_id = atoi(val);

					if (job_id < 0)
					{
						_cupsLangPrintf(stderr, _("%s: Error - bad job ID."), argv[0]);
						break;
					}
					break;

				case 'm': /* Send email when job is done */
				printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
#ifdef __sun
				case 'p': /* Notify on completion */
#endif					  /* __sun */
				case 'w': /* Write to console or email */
				{
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					char email[1024]; /* EMail address */

					snprintf(email, sizeof(email), "mailto:%s@%s", cupsUser(), httpGetHostname(NULL, buffer, sizeof(buffer)));
					num_options = cupsAddOption("notify-recipient-uri", email, num_options, &options);
				}

					silent = 1;
					break;

				case 'n': /* Number of copies */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						num_copies = atoi(opt + 1);
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected copies after \"-n\" option."), argv[0]);
							usage();
						}

						num_copies = atoi(argv[i]);
					}

					if (num_copies < 1)
					{
						_cupsLangPrintf(stderr, _("%s: Error - copies must be 1 or more."), argv[0]);
						return (1);
					}

					num_options = cupsAddIntegerOption("copies", num_copies, num_options, &options);
					break;

				case 'o': /* Option */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						num_options = cupsParseOptions(opt + 1, num_options, &options);
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected option=value after \"-o\" option."), argv[0]);
							usage();
						}

						num_options = cupsParseOptions(argv[i], num_options, &options);
					}
					break;

#ifndef __sun
				case 'p': /* Queue priority */
#endif					  /* !__sun */
				case 'q': /* Queue priority */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						priority = atoi(opt + 1);
						opt += strlen(opt) - 1;
					}
					else
					{
						if ((i + 1) >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected priority after \"-%c\" option."), argv[0], *opt);
							usage();
						}

						i++;

						priority = atoi(argv[i]);
					}

					/*
					 * For 100% Solaris compatibility, need to add:
					 *
					 *   priority = 99 * (39 - priority) / 39 + 1;
					 *
					 * However, to keep CUPS lp the same across all platforms
					 * we will break compatibility this far...
					 */

					if (priority < 1 || priority > 100)
					{
						_cupsLangPrintf(stderr, _("%s: Error - priority must be between 1 and 100."), argv[0]);
						return (1);
					}

					num_options = cupsAddIntegerOption("job-priority", priority, num_options, &options);
					break;

				case 's': /* Silent */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					silent = 1;
					break;

				case 't': /* Title */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						title = opt + 1;
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected title after \"-t\" option."), argv[0]);
							usage();
						}

						title = argv[i];
					}
					break;

				case 'y': /* mode-list */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected mode list after \"-y\" option."), argv[0]);
							usage();
						}
					}

					_cupsLangPrintf(stderr, _("%s: Warning - mode option ignored."), argv[0]);
					break;

				case 'H': /* Hold job */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						val = opt + 1;
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected hold name after \"-H\" option."), argv[0]);
							usage();
						}

						val = argv[i];
					}

					if (!strcmp(val, "hold"))
						num_options = cupsAddOption("job-hold-until", "indefinite", num_options, &options);
					else if (!strcmp(val, "resume") || !strcmp(val, "release"))
						num_options = cupsAddOption("job-hold-until", "no-hold", num_options, &options);
					else if (!strcmp(val, "immediate"))
					{
						num_options = cupsAddOption("job-hold-until", "no-hold", num_options, &options);
						num_options = cupsAddOption("job-priority", "100", num_options, &options);
					}
					else if (!strcmp(val, "restart"))
					{
						if (job_id < 1)
						{
							_cupsLangPrintf(stderr, _("%s: Need job ID (\"-i jobid\") before \"-H restart\"."), argv[0]);
							return (1);
						}

						if (restart_job(argv[0], job_id))
							return (1);
					}
					else
						num_options = cupsAddOption("job-hold-until", val, num_options, &options);
					break;

				case 'P': /* Page list */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						val = opt + 1;
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected page list after \"-P\" option."), argv[0]);
							usage();
						}

						val = argv[i];
					}

					num_options = cupsAddOption("page-ranges", val, num_options, &options);
					break;

				case 'S': /* character set */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected character set after \"-S\" option."), argv[0]);
							usage();
						}
					}

					_cupsLangPrintf(stderr, _("%s: Warning - character set option ignored."), argv[0]);
					break;

				case 'T': /* Content-Type */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						opt += strlen(opt) - 1;
					}
					else
					{
						i++;

						if (i >= argc)
						{
							_cupsLangPrintf(stderr, _("%s: Error - expected content type after \"-T\" option."), argv[0]);
							usage();
						}
					}

					_cupsLangPrintf(stderr, _("%s: Warning - content type option ignored."), argv[0]);
					break;

				case '-': /* Stop processing options */
					printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
					if (opt[1] != '\0')
					{
						_cupsLangPrintf(stderr, _("%s: Error - unknown option \"%s\"."), argv[0], argv[i]);
						usage();
					}

					end_options = 1;
					break;

				default:
					_cupsLangPrintf(stderr, _("%s: Error - unknown option \"%c\"."), argv[0], *opt);
					usage();
				}
			}
		}
		else if (!strcmp(argv[i], "-"))
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			if (num_files || job_id)
			{
				_cupsLangPrintf(stderr,
								_("%s: Error - cannot print from stdin if files or a "
								  "job ID are provided."),
								argv[0]);
				return (1);
			}

			break;
		}
		else if (num_files < 1000 && job_id == 0)
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			/*
			 * Print a file...
			 */

			if (access(argv[i], R_OK) != 0)
			{
				_cupsLangPrintf(stderr, _("%s: Error - unable to access \"%s\" - %s"), argv[0], argv[i], strerror(errno));
				return (1);
			}

			files[num_files] = argv[i];
			num_files++;

			if (title == NULL)
			{
				if ((title = strrchr(argv[i], '/')) != NULL)
					title++;
				else
					title = argv[i];
			}
		}
		else
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			_cupsLangPrintf(stderr, _("%s: Error - too many files - \"%s\"."), argv[0], argv[i]);
		}
	}

	/*
	 * See if we are altering an existing job...
	 */

	if (job_id)
	{
		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		return (set_job_attrs(argv[0], job_id, num_options, options));
	}

	/*
	 * See if we have any files to print; if not, print from stdin...
	 */
	printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
	if (printer == NULL)
	{
		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		if ((dest = cupsGetNamedDest(NULL, NULL, NULL)) != NULL)
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			printer = dest->name;

			for (j = 0; j < dest->num_options; j++)
				if (cupsGetOption(dest->options[j].name, num_options, options) == NULL)
					num_options = cupsAddOption(dest->options[j].name,
												dest->options[j].value,
												num_options, &options);
		}
		else if (cupsLastError() == IPP_STATUS_ERROR_BAD_REQUEST ||
				 cupsLastError() == IPP_STATUS_ERROR_VERSION_NOT_SUPPORTED)
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			_cupsLangPrintf(stderr,
							_("%s: Error - add '/version=1.1' to server "
							  "name."),
							argv[0]);
			return (1);
		}
	}

	printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
	if (printer == NULL)
	{
		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		if (!cupsGetNamedDest(NULL, NULL, NULL) && cupsLastError() == IPP_STATUS_ERROR_NOT_FOUND)
			_cupsLangPrintf(stderr, _("%s: Error - %s"), argv[0], cupsLastErrorString());
		else
			_cupsLangPrintf(stderr, _("%s: Error - scheduler not responding."), argv[0]);

		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		return (1);
	}

	printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
	if (num_files > 0)
	{
		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		job_id = cupsPrintFiles(printer, num_files, files, title, num_options, options);
	}
	else if ((job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, printer,
									 title ? title : "(stdin)",
									 num_options, options)) > 0)
	{
		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		http_status_t status; /* Write status */
		const char *format;	  /* Document format */
		ssize_t bytes;		  /* Bytes read */

		if (cupsGetOption("raw", num_options, options))
			format = CUPS_FORMAT_RAW;
		else if ((format = cupsGetOption("document-format", num_options,
										 options)) == NULL)
			format = CUPS_FORMAT_AUTO;

		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		status = cupsStartDocument(CUPS_HTTP_DEFAULT, printer, job_id, NULL,
								   format, 1);

		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		while (status == HTTP_CONTINUE &&
			   (bytes = read(0, buffer, sizeof(buffer))) > 0)
			status = cupsWriteRequestData(CUPS_HTTP_DEFAULT, buffer, (size_t)bytes);

		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		if (status != HTTP_CONTINUE)
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);

			_cupsLangPrintf(stderr, _("%s: Error - unable to queue from stdin - %s."),
							argv[0], httpStatus(status));
			cupsFinishDocument(CUPS_HTTP_DEFAULT, printer);
			cupsCancelJob2(CUPS_HTTP_DEFAULT, printer, job_id, 0);
			return (1);
		}

		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		if (cupsFinishDocument(CUPS_HTTP_DEFAULT, printer) != IPP_OK)
		{
			printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
			_cupsLangPrintf(stderr, "%s: %s", argv[0], cupsLastErrorString());
			cupsCancelJob2(CUPS_HTTP_DEFAULT, printer, job_id, 0);
			return (1);
		}
	}
	printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
	if (job_id < 1)
	{
		printf("Lien => File[%s] Line[%d] Function[%s]\n", __FILE__, __LINE__, __FUNCTION__);
		_cupsLangPrintf(stderr, "%s: %s", argv[0], cupsLastErrorString());
		return (1);
	}
	else if (!silent)
		_cupsLangPrintf(stdout, _("request id is %s-%d (%d file(s))"),
						printer, job_id, num_files);

	return (0);
}

/*
 * 'restart_job()' - Restart a job.
 */

static int						 /* O - Exit status */
restart_job(const char *command, /* I - Command name */
			int job_id)			 /* I - Job ID */
{
	ipp_t *request;			/* IPP request */
	char uri[HTTP_MAX_URI]; /* URI for job */

	request = ippNewRequest(IPP_RESTART_JOB);

	snprintf(uri, sizeof(uri), "ipp://localhost/jobs/%d", job_id);

	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
				 "job-uri", NULL, uri);

	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME,
				 "requesting-user-name", NULL, cupsUser());

	ippDelete(cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/jobs"));

	if (cupsLastError() == IPP_STATUS_ERROR_BAD_REQUEST ||
		cupsLastError() == IPP_STATUS_ERROR_VERSION_NOT_SUPPORTED)
	{
		_cupsLangPrintf(stderr,
						_("%s: Error - add '/version=1.1' to server "
						  "name."),
						command);
		return (1);
	}
	else if (cupsLastError() > IPP_OK_CONFLICT)
	{
		_cupsLangPrintf(stderr, "%s: %s", command, cupsLastErrorString());
		return (1);
	}

	return (0);
}

/*
 * 'set_job_attrs()' - Set job attributes.
 */

static int /* O - Exit status */
set_job_attrs(
	const char *command,	/* I - Command name */
	int job_id,				/* I - Job ID */
	int num_options,		/* I - Number of options */
	cups_option_t *options) /* I - Options */
{
	ipp_t *request;			/* IPP request */
	char uri[HTTP_MAX_URI]; /* URI for job */

	if (num_options == 0)
		return (0);

	request = ippNewRequest(IPP_SET_JOB_ATTRIBUTES);

	snprintf(uri, sizeof(uri), "ipp://localhost/jobs/%d", job_id);

	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
				 "job-uri", NULL, uri);

	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME,
				 "requesting-user-name", NULL, cupsUser());

	cupsEncodeOptions(request, num_options, options);

	ippDelete(cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/jobs"));

	if (cupsLastError() == IPP_STATUS_ERROR_BAD_REQUEST ||
		cupsLastError() == IPP_STATUS_ERROR_VERSION_NOT_SUPPORTED)
	{
		_cupsLangPrintf(stderr,
						_("%s: Error - add '/version=1.1' to server "
						  "name."),
						command);
		return (1);
	}
	else if (cupsLastError() > IPP_OK_CONFLICT)
	{
		_cupsLangPrintf(stderr, "%s: %s", command, cupsLastErrorString());
		return (1);
	}

	return (0);
}

/*
 * 'usage()' - Show program usage and exit.
 */

static void
usage(void)
{
	_cupsLangPuts(stdout, _("Usage: lp [options] [--] [file(s)]\n"
							"       lp [options] -i id"));
	_cupsLangPuts(stdout, _("Options:"));
	_cupsLangPuts(stdout, _("-c                      Make a copy of the print file(s)"));
	_cupsLangPuts(stdout, _("-d destination          Specify the destination"));
	_cupsLangPuts(stdout, _("-E                      Encrypt the connection to the server"));
	_cupsLangPuts(stdout, _("-h server[:port]        Connect to the named server and port"));
	_cupsLangPuts(stdout, _("-H HH:MM                Hold the job until the specified UTC time"));
	_cupsLangPuts(stdout, _("-H hold                 Hold the job until released/resumed"));
	_cupsLangPuts(stdout, _("-H immediate            Print the job as soon as possible"));
	_cupsLangPuts(stdout, _("-H restart              Reprint the job"));
	_cupsLangPuts(stdout, _("-H resume               Resume a held job"));
	_cupsLangPuts(stdout, _("-i id                   Specify an existing job ID to modify"));
	_cupsLangPuts(stdout, _("-m                      Send an email notification when the job completes"));
	_cupsLangPuts(stdout, _("-n num-copies           Specify the number of copies to print"));
	_cupsLangPuts(stdout, _("-o option[=value]       Specify a printer-specific option"));
	_cupsLangPuts(stdout, _("-o job-sheets=standard  Print a banner page with the job"));
	_cupsLangPuts(stdout, _("-o media=size           Specify the media size to use"));
	_cupsLangPuts(stdout, _("-o number-up=N          Specify that input pages should be printed N-up (1, 2, 4, 6, 9, and 16 are supported)"));
	_cupsLangPuts(stdout, _("-o orientation-requested=N\n"
							"                        Specify portrait (3) or landscape (4) orientation"));
	_cupsLangPuts(stdout, _("-o print-quality=N      Specify the print quality - draft (3), normal (4), or best (5)"));
	_cupsLangPuts(stdout, _("-o sides=one-sided      Specify 1-sided printing"));
	_cupsLangPuts(stdout, _("-o sides=two-sided-long-edge\n"
							"                        Specify 2-sided portrait printing"));
	_cupsLangPuts(stdout, _("-o sides=two-sided-short-edge\n"
							"                        Specify 2-sided landscape printing"));
	_cupsLangPuts(stdout, _("-P page-list            Specify a list of pages to print"));
	_cupsLangPuts(stdout, _("-q priority             Specify the priority from low (1) to high (100)"));
	_cupsLangPuts(stdout, _("-s                      Be silent"));
	_cupsLangPuts(stdout, _("-t title                Specify the job title"));
	_cupsLangPuts(stdout, _("-U username             Specify the username to use for authentication"));

	exit(1);
}
